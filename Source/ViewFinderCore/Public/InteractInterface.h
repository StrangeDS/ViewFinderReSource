#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

class VIEWFINDERCORE_API IInteractInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool StartAiming(APlayerController* Controller);
	virtual bool StartAiming_Implementation(APlayerController* Controller);


	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool EndAiming(APlayerController* Controller);
	virtual bool EndAiming_Implementation(APlayerController* Controller);
	
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool Interact(APlayerController* Controller);
	virtual bool Interact_Implementation(APlayerController* Controller);
};
