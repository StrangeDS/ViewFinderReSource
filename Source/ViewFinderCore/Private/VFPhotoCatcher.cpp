#include "VFPhotoCatcher.h"

#include "Components/StaticMeshComponent.h"

#include "VFPhotoCaptureComponent.h"
#include "VFDynamicMeshComponent.h"
#include "VFFunctions.h"

#include "Kismet/KismetSystemLibrary.h"

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

void AVFPhotoCatcher::TakeAPhoto_Implementation()
{
	TArray<UPrimitiveComponent *> OverlapComps;
	UKismetSystemLibrary::ComponentOverlapComponents(
		ViewFrustum,
		ViewFrustum->GetComponentToWorld(),
		ObjectTypesToOverlap,
		UPrimitiveComponent::StaticClass(),
		ActorsToIgnore,
		OverlapComps);
	
	if (OnlyOverlapWithHelps)
		UVFFunctions::FilterPrimCompsWithHelper(OverlapComps);

	UE_LOG(LogTemp, Warning, TEXT("TakeAPhoto_Implementation overlaps %i"), OverlapComps.Num());

	AVFPhoto3D *Photo = UVFFunctions::TakeAPhoto(ViewFrustum, OverlapComps);
}

void AVFPhotoCatcher::SetViewFrustumVisible(const bool &Visibility)
{
	ViewFrustum->SetHiddenInGame(!Visibility);
}
