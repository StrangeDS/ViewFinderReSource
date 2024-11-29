#include "PhotoCatcher_Interact.h"

#include "Blueprint/UserWidget.h"
#include "EnhancedInputSubsystems.h"

bool APhotoCatcher_Interact::StartAiming_Implementation(APlayerController *Controller)
{
	if (!AimingHintUMGClass.Get())
		return false;

	if (!AimingHintUMG)
		AimingHintUMG = CreateWidget<UUserWidget>(GetWorld(), AimingHintUMGClass, TEXT("HintUMG"));
	AimingHintUMG->AddToViewport();

	if (AimingMappingContext)
	{
		// 具体事件的BindAction放在蓝图中进行(图个方便)
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
			Subsystem->AddMappingContext(AimingMappingContext, 1);
	}
	return true;
}

bool APhotoCatcher_Interact::EndAiming_Implementation(APlayerController *Controller)
{
	if (AimingHintUMG)
		AimingHintUMG->RemoveFromParent();

	if (AimingMappingContext)
	{
		auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
		if (Subsystem)
			Subsystem->RemoveMappingContext(AimingMappingContext);
	}
	return true;
}

bool APhotoCatcher_Interact::Interact_Implementation(APlayerController *Controller)
{
	TakeAPhoto();
	return true;
}

bool APhotoCatcher_Interact::IsEnabled_Implementation(APlayerController *Controller)
{
	return bInteractingEnabled;
}

void APhotoCatcher_Interact::EnableInteract_Implementation(bool Enabled)
{
	if (bInteractingEnabled == Enabled)
		return;

	bInteractingEnabled = Enabled;
	if (bInteractingEnabled)
	{
		CollisionEnabledRecord = StaticMesh->GetCollisionEnabled();
		StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
	else
	{
		StaticMesh->SetCollisionEnabled(CollisionEnabledRecord);
	}
}

void APhotoCatcher_Interact::CloseToPreview_Implementation()
{
	if (!Pawn || !PlayerController) {
		UE_LOG(LogTemp, Warning, TEXT("CloseToPreview_Implementation."));
		return;
	}

	GetWorldTimerManager().SetTimer(PreviewTimeHanlde, [this]()
									{ bReady = true; }, TimeOfClose, false);
	Pawn->DisableInput(PlayerController);
	PlayerController->SetViewTargetWithBlend(this, TimeOfClose);
}

void APhotoCatcher_Interact::LeaveFromPreview_Implementation()
{
	if (!Pawn || !PlayerController) {
		
		UE_LOG(LogTemp, Warning, TEXT("LeaveFromPreview_Implementati"));
		return;
	}

	PreviewTimeHanlde.Invalidate();
	bReady = false;
	PlayerController->SetViewTargetWithBlend(Pawn, TimeOfLeave);
	Pawn->EnableInput(PlayerController);
}