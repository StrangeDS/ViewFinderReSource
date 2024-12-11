#include "VFPhotoCatcher.h"

#include "Components/StaticMeshComponent.h"
#include "VFDynamicMeshComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "VFPhoto2D.h"
#include "VFPhoto3D.h"
#include "VFPhotoCaptureComponent.h"
#include "VFFunctions.h"

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

// #if WITH_EDITOR
// void AVFPhotoCatcher::PostEditChangeProperty(FPropertyChangedEvent &Event)
// {
// 	const FName PropertyName = Event.Property ? Event.Property->GetFName(): NAME_None;
// 	if (PropertyName == GET_MEMBER_NAME_CHECKED(AVFPhotoCatcher, ViewAngle)
// 	|| PropertyName == GET_MEMBER_NAME_CHECKED(AVFPhotoCatcher, AspectRatio)
// 	|| PropertyName == GET_MEMBER_NAME_CHECKED(AVFPhotoCatcher, StartDis)
// 	|| PropertyName == GET_MEMBER_NAME_CHECKED(AVFPhotoCatcher, EndDis))
// 	{
// 		ViewFrustum->RegenerateViewFrustum(ViewAngle, AspectRatio, StartDis, EndDis);
// 	}

// 	Super::PostEditChangeProperty(Event);
// }
// #endif

void AVFPhotoCatcher::BeginPlay()
{
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
	TArray<UPrimitiveComponent *> OverlapComps;
	UKismetSystemLibrary::ComponentOverlapComponents(
		ViewFrustum,
		ViewFrustum->GetComponentToWorld(),
		ObjectTypesToOverlap,
		UPrimitiveComponent::StaticClass(),
		ActorsToIgnore,
		OverlapComps);
	auto VFDMComps = UVFFunctions::CheckVFDMComps(OverlapComps);

	TMap<UVFDynamicMeshComponent *, UVFHelperComponent *> HelperMap;
	UVFFunctions::GetCompsToHelpersMapping(VFDMComps, HelperMap);
	for (auto It = VFDMComps.CreateIterator(); It; It++)
	{
		auto Comp = *It;
		auto Helper = HelperMap[Comp];
		if (bOnlyOverlapWithHelps && !Helper)
			It.RemoveCurrent();
		else if (Helper && !HelperMap[Comp]->bCanBePlacedByPhoto)
			It.RemoveCurrent();
	}
	UE_LOG(LogTemp, Warning, TEXT("TakeAPhoto_Implementation overlaps %i"), OverlapComps.Num());

	TArray<UVFDynamicMeshComponent *> CopiedComps;

	AVFPhoto3D *Photo3D = GetWorld()->SpawnActor<AVFPhoto3D>(
		ViewFrustum->GetComponentLocation(),
		ViewFrustum->GetComponentRotation());
	auto ActorsCopied = UVFFunctions::CopyActorFromVFDMComps(Photo3D, VFDMComps, CopiedComps);

	if (bCuttingSource)
	{
		for (auto &Comp : VFDMComps)
		{
			UVFFunctions::SubtractWithFrustum(Comp, ViewFrustum);
		}
	}

	for (auto &Comp : CopiedComps)
	{
		UVFFunctions::IntersectWithFrustum(Comp, ViewFrustum);
	}

	AVFPhoto2D *Photo2D = GetWorld()->SpawnActor<AVFPhoto2D>(
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
