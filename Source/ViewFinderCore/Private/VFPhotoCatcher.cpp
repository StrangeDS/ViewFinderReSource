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
	StaticMesh->SetCollisionProfileName(TEXT("OverlapOnlyPawn"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSelector(
		TEXT("/Game/ViewFinder/StaticMesh/Camera_Temp.Camera_Temp"));
	CatcherMesh = MeshSelector.Object;
	StaticMesh->SetStaticMesh(CatcherMesh);

	PhotoCapture = CreateDefaultSubobject<UVFPhotoCaptureComponent>(TEXT("PhotoCapture"));
	PhotoCapture->SetupAttachment(RootComponent);

	ViewFrustum = CreateDefaultSubobject<UVFViewFrustumComponent>(TEXT("ViewFrustum"));
	ViewFrustum->SetupAttachment(RootComponent);
	ViewFrustum->SetHiddenInGame(true);
}

void AVFPhotoCatcher::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	ActorsToIgnore.AddUnique(this);
	ViewFrustum->RegenerateViewFrustum(ViewAngle, AspectRatio, StartDis, EndDis);
	PhotoCapture->FOVAngle = ViewAngle;
	PhotoCapture->Init(StaticMesh->CreateDynamicMaterialInstance(0), AspectRatio);
	PhotoCapture->CustomNearClippingPlane = StartDis;
}

void AVFPhotoCatcher::BeginPlay()
{
	check(VFDMCompClass.Get());
	check(VFPhoto2DClass.Get());
	check(VFPhoto3DClass.Get());
	check(VFPawnStandInClass.Get());

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
	
	// 对Pawn的特殊处理
	UVFFunctions::CheckPawnComps(OverlapComps, VFPawnStandInClass, true);

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
	auto ActorsCopied = UVFFunctions::CopyActorFromVFDMComps(GetWorld(), VFDMComps, CopiedComps);
	for (auto &Actor : ActorsCopied)
	{
		Actor->AttachToActor(Photo3D, FAttachmentTransformRules::KeepWorldTransform);
	}

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
	Photo3D->RecordProperty(ViewFrustum, bOnlyOverlapWithHelps, ObjectTypesToOverlap);
	Photo2D->FoldUp();

	Photo2D->AspectRatio = AspectRatio;
	Photo2D->SetPhoto(PhotoCapture);

	for (auto &Helper : HelpersRecorder)
	{
		Helper->NotifyDelegate(FVFHelperDelegateType::OriginalAfterTakingPhoto);
	}

	return Photo2D;
}

void AVFPhotoCatcher::SetViewFrustumVisible(const bool &Visibility)
{
	ViewFrustum->SetHiddenInGame(!Visibility);
}
