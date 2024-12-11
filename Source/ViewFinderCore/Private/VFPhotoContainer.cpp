#include "VFPhotoContainer.h"

#include "VFPhoto2D.h"


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

void AVFPhotoContainer::AddAPhoto(AVFPhoto2D *Photo)
{
	Photo2Ds.EmplaceLast(Photo);

	if (!CurrentPhoto2D && !Photo2Ds.IsEmpty())
		CurrentPhoto2D = Photo2Ds.Last();
}

void AVFPhotoContainer::CreateAPhoto(const FTransform &WorldTrans)
{
	// 需要缩放吗?
	AVFPhoto2D *Photo = GetWorld()->SpawnActor<AVFPhoto2D>(
		WorldTrans.GetLocation(),
		WorldTrans.Rotator());
	AddAPhoto(Photo);
}

void AVFPhotoContainer::PrepareCurrentPhoto(float Time)
{
	if (!CurrentPhoto2D)
		return;

	Time = Time >= 0.f ? Time : TimeOfSelect;
	GetWorldTimerManager().SetTimer(
		PrepareTimeHandle, [this]()
		{
            bFocusOn = true;
            CurrentPhoto2D->Preview(true); },
		Time,
		false);
}

void AVFPhotoContainer::GiveUpPrepare()
{
    GetWorldTimerManager().ClearTimer(PrepareTimeHandle);
    bFocusOn = false;
    CurrentPhoto2D->Preview(false);
}

void AVFPhotoContainer::PlaceCurrentPhoto()
{
	CurrentPhoto2D->PlaceDown();
	ChangeCurrentPhoto(false);
}

void AVFPhotoContainer::ChangeCurrentPhoto(const bool &Next)
{
	if (Photo2Ds.Num() <= 1)
		return;

	if (Next)
	{
		auto Photo = Photo2Ds.First();
		Photo2Ds.PopFirst();
		Photo2Ds.EmplaceLast();
	}
	else
	{
		auto Photo = Photo2Ds.Last();
		Photo2Ds.PopLast();
		Photo2Ds.EmplaceFirst();
	}
	UpdateCurrentPhoto();
}

void AVFPhotoContainer::UpdateCurrentPhoto()
{
	CurrentPhoto2D = Photo2Ds.IsEmpty() ? nullptr : Photo2Ds.Last();
}
