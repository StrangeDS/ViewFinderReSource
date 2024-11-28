#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "StepsRecordInterface.generated.h"

USTRUCT(BlueprintType)
struct FStepObjectInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UObject> Object;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	FString Info;
};

USTRUCT(BlueprintType)
struct FStepInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float Time = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<FStepObjectInfo> Events;
};

UINTERFACE(MinimalAPI)
class UStepsRecordInterface : public UInterface
{
	GENERATED_BODY()
};

class VIEWFINDERCORE_API IStepsRecordInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	FStepInfo MakeStepInfo();
	virtual FStepInfo MakeStepInfo_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool StepBack(FString &Step);
	virtual bool StepBack_Implementation(FString &Step);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool WhetherToTick();
	virtual bool WhetherToTick_Implementation();
};
