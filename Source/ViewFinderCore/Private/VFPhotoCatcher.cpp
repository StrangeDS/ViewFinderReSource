#include "VFPhotoCatcher.h"

#include "Components/StaticMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "VFDynamicMeshComponent.h"
#include "VFPhoto2D.h"
#include "VFPhoto3D.h"
#include "VFPhotoCaptureComponent.h"
#include "VFViewFrustumComponent.h"
#include "VFFunctions.h"

static void GetMapHelpers(
	const TMap<UPrimitiveComponent *, UVFHelperComponent *> &Map,
	TSet<UVFHelperComponent *> &Helpers)
{
	Helpers.Reset();
	for (auto &[Comp, Helper] : Map)
	{
		Helpers.Add(Helper);
	}
}

AVFPhotoCatcher::AVFPhotoCatcher()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	PhotoCapture = CreateDefaultSubobject<UVFPhotoCaptureComponent>(TEXT("PhotoCapture"));
	PhotoCapture->SetupAttachment(RootComponent);
	ViewFrustum = CreateDefaultSubobject<UVFViewFrustumComponent>(TEXT("ViewFrustum"));
	ViewFrustum->SetupAttachment(RootComponent);
}

void AVFPhotoCatcher::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	ActorsToIgnore.AddUnique(this);
	ViewFrustum->RegenerateViewFrustum(ViewAngle, AspectRatio, StartDis, EndDis);
	// TODO: Sync PhotoCapture.
	PhotoCapture->Init(StaticMesh);
}

void AVFPhotoCatcher::BeginPlay()
{
	check(VFDMCompClass.Get());
	check(VFPhoto2DClass.Get());
	check(VFPhoto3DClass.Get());

	Super::BeginPlay();

	SetViewFrustumVisible(false);
}

void AVFPhotoCatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

AVFPhoto2D *AVFPhotoCatcher::TakeAPhoto_Implementation()
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("AVFPhotoCatcher::TakeAPhoto_Implementation()"));

	// 重叠检测
	TArray<UPrimitiveComponent *> OverlapComps;
	UKismetSystemLibrary::ComponentOverlapComponents(
		ViewFrustum,
		ViewFrustum->GetComponentToWorld(),
		ObjectTypesToOverlap,
		UPrimitiveComponent::StaticClass(),
		ActorsToIgnore,
		OverlapComps);

	// Helper筛选
	TMap<UPrimitiveComponent *, UVFHelperComponent *> HelperMap;
	UVFFunctions::GetCompsToHelpersMapping<UPrimitiveComponent>(OverlapComps, HelperMap);

	PhotoCapture->HiddenComponents.Reset();
	for (auto It = OverlapComps.CreateIterator(); It; It++)
	{
		auto Comp = *It;
		auto Helper = HelperMap.Find(Comp); // 可能为nullptr
		if (bOnlyOverlapWithHelps && !Helper)
		{
			PhotoCapture->HiddenComponents.AddUnique(Comp);
			It.RemoveCurrent();
		}
		else if (Helper && !HelperMap[Comp]->bCanBeTakenInPhoto)
		{
			PhotoCapture->HiddenComponents.AddUnique(Comp);
			It.RemoveCurrent();
		}
	}

	TSet<UVFHelperComponent *> HelpersRecorder;
	GetMapHelpers(HelperMap, HelpersRecorder);

	for (auto &Helper : HelpersRecorder)
	{
		Helper->NotifyDelegate(FVFHelperDelegateType::OriginalBeforeTakenInPhoto);
	}

	// 基元组件下创建对应VFDynamicMeshComponent
	auto VFDMComps = UVFFunctions::CheckVFDMComps(OverlapComps, VFDMCompClass);

	// 需要排序吗?
	// Algo::Sort(VFDMComps, [](UVFDynamicMeshComponent *A, UVFDynamicMeshComponent *B)
	// 			{ return A->GetOwner()->GetName() < B->GetOwner()->GetName(); });
	// for (auto Comp: VFDMComps)
	// {
	// 	UE_LOG(LogTemp, Warning, TEXT("%s"), *Comp->GetOwner()->GetName());
	// }

	UE_LOG(LogTemp, Warning, TEXT("TakeAPhoto_Implementation overlaps %i"), VFDMComps.Num());
	
	for (auto &Helper : HelpersRecorder)
	{
		Helper->NotifyDelegate(FVFHelperDelegateType::OriginalBeforeCopyingToPhoto);
	}
	// 复制对应Actor
	TArray<UVFDynamicMeshComponent *> CopiedComps;
	AVFPhoto3D *Photo3D = GetWorld()->SpawnActor<AVFPhoto3D>(
		VFPhoto3DClass.Get(),
		ViewFrustum->GetComponentLocation(),
		ViewFrustum->GetComponentRotation());
	auto ActorsCopied = UVFFunctions::CopyActorFromVFDMComps(Photo3D, VFDMComps, CopiedComps);

	TMap<UPrimitiveComponent *, UVFHelperComponent *> CopiedHelperMap;
	UVFFunctions::GetCompsToHelpersMapping<UVFDynamicMeshComponent>(CopiedComps, CopiedHelperMap);
	TSet<UVFHelperComponent *> CopiedHelpersRecorder;
	GetMapHelpers(CopiedHelperMap, CopiedHelpersRecorder);

	for (auto &Helper : CopiedHelpersRecorder)
	{
		Helper->NotifyDelegate(FVFHelperDelegateType::CopyAfterCopiedForPhoto);
	}

	// 原VFDynamicMeshComponent做切割
	if (bCuttingOrignal)
	{
		for (auto &Comp : VFDMComps)
		{
			Comp->SubtractMeshWithDMComp(ViewFrustum);
		}
		for (auto &Helper : HelpersRecorder)
		{
			Helper->NotifyDelegate(FVFHelperDelegateType::OriginalAfterCutByPhoto);
		}
	}

	// 新VFDynamicMeshComponent做交集
	for (auto &Comp : CopiedComps)
	{
		Comp->IntersectMeshWithDMComp(ViewFrustum);
	}
	for (auto &Helper : CopiedHelpersRecorder)
	{
		Helper->NotifyDelegate(FVFHelperDelegateType::CopyBeforeFoldedInPhoto);
	}

	// 创建照片, 后续处理
	AVFPhoto2D *Photo2D = GetWorld()->SpawnActor<AVFPhoto2D>(
		VFPhoto2DClass.Get(),
		ViewFrustum->GetComponentLocation(),
		ViewFrustum->GetComponentRotation());
	Photo2D->SetPhoto3D(Photo3D);
	Photo2D->SetPhoto(PhotoCapture);
	Photo3D->RecordProperty(ViewFrustum, bOnlyOverlapWithHelps, ObjectTypesToOverlap);
	Photo2D->FoldUp();

	return Photo2D;
}

void AVFPhotoCatcher::SetViewFrustumVisible(const bool &Visibility)
{
	ViewFrustum->SetHiddenInGame(!Visibility);
}
