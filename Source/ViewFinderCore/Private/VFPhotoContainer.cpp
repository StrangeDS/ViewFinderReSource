#include "VFPhotoContainer.h"

#include "Kismet/GameplayStatics.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

#include "VFPhoto2D.h"

AVFPhotoContainer::AVFPhotoContainer()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	Container = CreateDefaultSubobject<USceneComponent>(TEXT("Container"));
	Container->SetupAttachment(RootComponent);

    static ConstructorHelpers::FObjectFinder<UInputMappingContext> Selector(
		TEXT("/Game/ViewFinder/Input/IMC_PhotoContainer.IMC_PhotoContainer")
		);
	UE_LOG(LogTemp, Warning, TEXT("Selector: %i"), Selector.Succeeded());
    MappingContext = Selector.Object;
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
	Photo->AttachToComponent(Container, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	UpdateCurrentPhoto();
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

void AVFPhotoContainer::GiveUpPreparing()
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

void AVFPhotoContainer::ChangeCurrentPhoto(const bool Next)
{
	if (Photo2Ds.Num() <= 1)
		return;

	if (bFocusOn)
		return;

	if (Next)
	{
		auto Photo = Photo2Ds.First();
		Photo2Ds.PopFirst();
		Photo2Ds.EmplaceLast(Photo);
	}
	else
	{
		auto Photo = Photo2Ds.Last();
		Photo2Ds.PopLast();
		Photo2Ds.EmplaceFirst(Photo);
	}
	UpdateCurrentPhoto();
	SetEnabled(true);
}

void AVFPhotoContainer::UpdateCurrentPhoto()
{
	if (CurrentPhoto2D)
		CurrentPhoto2D->SetActorHiddenInGame(true);

	CurrentPhoto2D = Photo2Ds.IsEmpty() ? nullptr : Photo2Ds.Last();

	if (CurrentPhoto2D)
	{
		CurrentPhoto2D->SetActorHiddenInGame(false);
	}
	else{
		SetEnabled(false);
	}
}

void AVFPhotoContainer::SetEnabled(const bool &Enabled)
{
	if (bEnabled == Enabled)
		return;
	bEnabled = Enabled;

	if (Num() <= 0)
		bEnabled = false;

	check(PlayerController);

	if (bEnabled)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
			Subsystem->AddMappingContext(MappingContext, 2);
        EnableInput(PlayerController);
		SetActorHiddenInGame(false);
	}
	else
	{
        DisableInput(PlayerController);
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
			Subsystem->RemoveMappingContext(MappingContext);
		SetActorHiddenInGame(true);
	}
}
