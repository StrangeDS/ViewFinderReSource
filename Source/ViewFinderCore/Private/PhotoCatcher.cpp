#include "PhotoCatcher.h"

#include "Components/StaticMeshComponent.h"

#include "PhotoCaptureComponent.h"
#include "ViewFrustumComponent.h"
#include "VFDynamicMeshComponent.h"
#include "ViewFinderFunctions.h"

#include "Kismet/KismetSystemLibrary.h"

APhotoCatcher::APhotoCatcher()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	PhotoCapture = CreateDefaultSubobject<UPhotoCaptureComponent>(TEXT("PhotoCapture"));
	PhotoCapture->SetupAttachment(RootComponent);
	ViewFrustum = CreateDefaultSubobject<UViewFrustumComponent>(TEXT("ViewFrustum"));
	ViewFrustum->SetupAttachment(RootComponent);
	
}

void APhotoCatcher::OnConstruction(const FTransform &Transform)
{
	Super::OnConstruction(Transform);

	ActorsToIgnore.AddUnique(this);
	ViewFrustum->RegenerateViewFrustum(ViewAngle, AspectRatio, StartDis, EndDis);
	// TODO: Sync PhotoCapture.
}

// #if WITH_EDITOR
// void APhotoCatcher::PostEditChangeProperty(FPropertyChangedEvent &Event)
// {
// 	const FName PropertyName = Event.Property ? Event.Property->GetFName(): NAME_None;
// 	if (PropertyName == GET_MEMBER_NAME_CHECKED(APhotoCatcher, ViewAngle)
// 	|| PropertyName == GET_MEMBER_NAME_CHECKED(APhotoCatcher, AspectRatio)
// 	|| PropertyName == GET_MEMBER_NAME_CHECKED(APhotoCatcher, StartDis)
// 	|| PropertyName == GET_MEMBER_NAME_CHECKED(APhotoCatcher, EndDis))
// 	{
// 		ViewFrustum->RegenerateViewFrustum(ViewAngle, AspectRatio, StartDis, EndDis);
// 	}

// 	Super::PostEditChangeProperty(Event);
// }
// #endif

void APhotoCatcher::BeginPlay()
{
	Super::BeginPlay();

	SetViewFrustumVisible(false);
}

void APhotoCatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhotoCatcher::TakeAPhoto_Implementation()
{
	TArray<UPrimitiveComponent *> OverlapComps;
	UKismetSystemLibrary::ComponentOverlapComponents(
		ViewFrustum,
		ViewFrustum->GetComponentToWorld(),
		ObjectTypesToOverlap,
		UPrimitiveComponent::StaticClass(),
		ActorsToIgnore,
		OverlapComps);
	UE_LOG(LogTemp, Warning, TEXT("TakeAPhoto_Implementation overlaps %i"), OverlapComps.Num());
	// TArray<UVFDynamicMeshComponent *> VFDMComps = UViewFinderFunctions::CheckVFDMComps(OverlapComps);
	// TArray<AActor *> Copied = UViewFinderFunctions::CopyActorFromVFDMComps(VFDMComps);
}

void APhotoCatcher::SetViewFrustumVisible(const bool &Visibility)
{
	check(ViewFrustum);
	ViewFrustum->SetHiddenInGame(!Visibility);
}
