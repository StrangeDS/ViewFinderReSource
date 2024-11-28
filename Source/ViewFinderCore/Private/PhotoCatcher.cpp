#include "PhotoCatcher.h"

#include "Blueprint/UserWidget.h"
#include "Components/StaticMeshComponent.h"

#include "PhotoCaptureComponent.h"
#include "ViewFrustumComponent.h"

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
	
}

void APhotoCatcher::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

bool APhotoCatcher::StartAiming_Implementation(APlayerController *Controller)
{
	if (!HintUMGClass.Get())
    	return false;

	if (!HintUMG) {
		HintUMG = CreateWidget<UUserWidget>(GetWorld(), HintUMGClass, TEXT("HintUMG"));
	}
	HintUMG->AddToViewport();
	return true;
}

bool APhotoCatcher::EndAiming_Implementation(APlayerController *Controller)
{
	if (HintUMG) HintUMG->RemoveFromParent();
    return true;
}

bool APhotoCatcher::Interact_Implementation(APlayerController *Controller)
{
    return false;
}
