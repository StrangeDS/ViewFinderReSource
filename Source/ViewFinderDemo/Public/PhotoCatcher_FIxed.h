#pragma once

#include "CoreMinimal.h"
#include "PhotoCatcher_Interact.h"
#include "PhotoCatcher_Fixed.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERDEMO_API APhotoCatcher_Fixed : public APhotoCatcher_Interact
{
	GENERATED_BODY()
	
public:
	virtual bool StartAiming_Implementation(APlayerController *Controller) override;

	virtual bool EndAiming_Implementation(APlayerController *Controller) override;

	virtual bool Interact_Implementation(APlayerController *Controller) override;
};
