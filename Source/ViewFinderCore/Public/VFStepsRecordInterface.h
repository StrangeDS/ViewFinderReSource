#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "VFStepsRecordInterface.generated.h"

USTRUCT(BlueprintType)
struct FVFStepObjectInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UObject> Object;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	FString Info;
};

USTRUCT(BlueprintType)
struct FVFStepInfo
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float Time = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<FVFStepObjectInfo> Events;
};

UINTERFACE(MinimalAPI)
class UVFStepsRecordInterface : public UInterface
{
	GENERATED_BODY()
};

class VIEWFINDERCORE_API IVFStepsRecordInterface
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	FVFStepInfo MakeStepInfo();
	virtual FVFStepInfo MakeStepInfo_Implementation();

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool StepBack(FString &Step);
	virtual bool StepBack_Implementation(FString &Step);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	bool WhetherToTick();
	virtual bool WhetherToTick_Implementation();
};
