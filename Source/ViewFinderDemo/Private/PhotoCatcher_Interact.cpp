#include "PhotoCatcher_Interact.h"

#include "Blueprint/UserWidget.h"

bool APhotoCatcher_Interact::StartAiming_Implementation(APlayerController *Controller)
{
	if (!HintUMGClass.Get())
    	return false;

	if (!HintUMG) {
		HintUMG = CreateWidget<UUserWidget>(GetWorld(), HintUMGClass, TEXT("HintUMG"));
	}
	HintUMG->AddToViewport();
	return true;
}

bool APhotoCatcher_Interact::EndAiming_Implementation(APlayerController *Controller)
{
	if (HintUMG) HintUMG->RemoveFromParent();
    return true;
}

bool APhotoCatcher_Interact::Interact_Implementation(APlayerController *Controller)
{
    return false;
}