#include "VFPhotoCatcher_Fixed.h"

bool AVFPhotoCatcher_Fixed::StartAiming_Implementation(APlayerController *Controller)
{
    Super::StartAiming_Implementation(Controller);

	SetViewFrustumVisible(true);
    return true;
}

bool AVFPhotoCatcher_Fixed::EndAiming_Implementation(APlayerController *Controller)
{
    Super::EndAiming_Implementation(Controller);

	SetViewFrustumVisible(false);
    return true;
}

bool AVFPhotoCatcher_Fixed::Interact_Implementation(APlayerController *Controller)
{
	TakeAPhoto();
    return true;
}
