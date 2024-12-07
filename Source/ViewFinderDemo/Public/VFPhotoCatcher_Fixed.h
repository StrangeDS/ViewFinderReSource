#pragma once

#include "CoreMinimal.h"
#include "VFPhotoCatcher_Interact.h"
#include "VFPhotoCatcher_Fixed.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERDEMO_API AVFPhotoCatcher_Fixed : public AVFPhotoCatcher_Interact
{
	GENERATED_BODY()
	
public:
	virtual bool StartAiming_Implementation(APlayerController *Controller) override;

	virtual bool EndAiming_Implementation(APlayerController *Controller) override;

	virtual bool Interact_Implementation(APlayerController *Controller) override;
};
