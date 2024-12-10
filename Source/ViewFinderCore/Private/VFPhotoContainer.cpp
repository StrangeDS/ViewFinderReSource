#include "VFPhotoContainer.h"

#include "VFPhoto3D.h"


AVFPhotoContainer::AVFPhotoContainer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Container = CreateDefaultSubobject<USceneComponent>(TEXT("Container"));
	Container->SetupAttachment(RootComponent);
}

void AVFPhotoContainer::BeginPlay()
{
	Super::BeginPlay();
}

void AVFPhotoContainer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVFPhotoContainer::AddAPhoto(AVFPhoto3D *Photo)
{
	Photos.EmplaceLast(Photo);

	if (!CurrentPhoto && !Photos.IsEmpty())
		CurrentPhoto = Photos.Last();
}

void AVFPhotoContainer::CreateAPhoto(const FTransform &WorldTrans)
{
	// 需要缩放吗?
	AVFPhoto3D *Photo = GetWorld()->SpawnActor<AVFPhoto3D>(
		WorldTrans.GetLocation(),
		WorldTrans.Rotator());
	AddAPhoto(Photo);
}

void AVFPhotoContainer::PrepareCurrentPhoto(float Time)
{
	if (!CurrentPhoto)
		return;

	Time = Time >= 0.f ? Time : TimeOfSelect;
	GetWorldTimerManager().SetTimer(
		PrepareTimeHandle, [this]()
		{
            bFocusOn = true;
            CurrentPhoto->SetViewFrustumVisible(true); },
		Time,
		false);
}

void AVFPhotoContainer::GiveUpPrepare()
{
    GetWorldTimerManager().ClearTimer(PrepareTimeHandle);
    bFocusOn = false;
    CurrentPhoto->SetViewFrustumVisible(false);
}

void AVFPhotoContainer::PlaceCurrentPhoto()
{
	CurrentPhoto->PlaceDown();
	ChangeCurrentPhoto(false);
}

void AVFPhotoContainer::ChangeCurrentPhoto(const bool &Next)
{
	if (Photos.Num() <= 1)
		return;

	if (Next)
	{
		auto Photo = Photos.First();
		Photos.PopFirst();
		Photos.EmplaceLast();
	}
	else
	{
		auto Photo = Photos.Last();
		Photos.PopLast();
		Photos.EmplaceFirst();
	}
	UpdateCurrentPhoto();
}

void AVFPhotoContainer::UpdateCurrentPhoto()
{
	CurrentPhoto = Photos.IsEmpty() ? nullptr : Photos.Last();
}
