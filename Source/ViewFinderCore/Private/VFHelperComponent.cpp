#include "VFHelperComponent.h"

UVFHelperComponent::UVFHelperComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}


void UVFHelperComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UVFHelperComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UVFHelperComponent::BeginActorTakenInPhoto()
{
	ActorBeginTakenInPhoto.Broadcast();
}

void UVFHelperComponent::EndActorTakenInPhoto()
{
	EndTakenInPhotoSignature.Broadcast();
}

void UVFHelperComponent::BeginActorPlacedFromPhoto()
{
	BeginPlacedFromPhotoSignature.Broadcast();
}

void UVFHelperComponent::EndActorPlacedFromPhoto()
{
	EndPlacedFromPhotoSignature.Broadcast();
}
