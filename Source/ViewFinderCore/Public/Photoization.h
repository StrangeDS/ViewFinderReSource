#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Photoization.generated.h"

UINTERFACE(MinimalAPI)
class UPhotoization : public UInterface
{
	GENERATED_BODY()
};

class VIEWFINDERCORE_API IPhotoization
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool CanBeTaken();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	bool IsHidden();
};
