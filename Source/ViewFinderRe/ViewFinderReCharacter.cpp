// Copyright Epic Games, Inc. All Rights Reserved.

#include "ViewFinderReCharacter.h"
#include "ViewFinderReProjectile.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "Engine/LocalPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "VFHelperComponent.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

//////////////////////////////////////////////////////////////////////////
// AViewFinderReCharacter

AViewFinderReCharacter::AViewFinderReCharacter()
{
	// Character doesnt have a rifle at start
	bHasRifle = false;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	// Mesh1P->SetRelativeRotation(FRotator(0.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	Helper = CreateDefaultSubobject<UVFHelperComponent>("Helper");
	Helper->bCanBePlacedByPhoto = false;
}

void AViewFinderReCharacter::BeginPlay()
{
	// Call the base class
	Super::BeginPlay();

	// Add Input Mapping Context
	if (APlayerController *PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem *Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	StepRecorder = GetWorld()->GetSubsystem<UVFStepsRecorderWorldSubsystem>();
	check(StepRecorder);
	StepRecorder->RegisterTickable(this);
}

//////////////////////////////////////////////////////////////////////////// Input

void AViewFinderReCharacter::SetupPlayerInputComponent(UInputComponent *PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent *EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AViewFinderReCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AViewFinderReCharacter::Look);
	}
	else
	{
		UE_LOG(LogTemplateCharacter, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}
}

void AViewFinderReCharacter::Move(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void AViewFinderReCharacter::Look(const FInputActionValue &Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AViewFinderReCharacter::SetHasRifle(bool bNewHasRifle)
{
	bHasRifle = bNewHasRifle;
}

bool AViewFinderReCharacter::GetHasRifle()
{
	return bHasRifle;
}

FVFPawnTransformInfo::FVFPawnTransformInfo(APawn *Pawn, float TimeIn)
	: Location(Pawn->GetActorLocation()),
	  Velocity(Pawn->GetVelocity()),
	  Rotator(Pawn->GetViewRotation()),
	  Time(TimeIn)
{
}

bool FVFPawnTransformInfo::operator==(const FVFPawnTransformInfo &Other) const
{
	if (Location != Other.Location)
		return false;
	if (Velocity != Other.Velocity)
		return false;
	return Rotator == Other.Rotator;
}

void AViewFinderReCharacter::TickForward_Implementation(float Time)
{
	FVFPawnTransformInfo Info(this, StepRecorder->Time);
	if (Steps.IsEmpty() || Steps.Last() != Info)
	{
		Steps.Add(Info);
	}
}

void AViewFinderReCharacter::TickBackward_Implementation(float Time)
{
	while (Steps.Num() > 1)
	{
		auto &StepInfo = Steps.Last();
		if (StepInfo.Time < Time)
			break;

		Steps.Pop(false);
	}

	auto &Step = Steps.Last();
	auto Delta = StepRecorder->GetDeltaTime() / (StepRecorder->GetTime() - Step.Time);
	Delta = FMath::Min(Delta, 1.0f);
	SetActorLocation(FMath::Lerp(GetActorLocation(), Step.Location, Delta));

	auto Velocity = FMath::Lerp(GetVelocity(), Step.Velocity, Delta);
	if (GetRootComponent() && GetRootComponent()->IsSimulatingPhysics())
	{
		GetRootComponent()->ComponentVelocity = Velocity;
	}
	else
	{
		GetCharacterMovement()->Velocity = Velocity;
	}

	GetController()->SetControlRotation(FMath::Lerp(GetViewRotation(), Step.Rotator, Delta));
}