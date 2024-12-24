#pragma once

#include "CoreMinimal.h"
#include "VFDynamicMeshComponent.h"

#include "VFStepsRecordInterface.h"
#include "VFStepsRecorderWorldSubsystem.h"

#include "VFDMSteppableComponent.generated.h"

UENUM(BlueprintType)
enum class UVFDMSteppableCompStep
{
	BeginPlay = 0,
	CopyMeshFromComponent = 1,
	RegisterToTransformRecorder = 2
};

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API UVFDMSteppableComponent : public UVFDynamicMeshComponent, public IVFStepsRecordInterface
{
	GENERATED_BODY()

public:
	UVFDMSteppableComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;

public:
	virtual void CopyMeshFromComponent(UPrimitiveComponent* Source) override;

public:
	virtual bool StepBack_Implementation(FVFStepInfo &StepInfo) override;
	
	// 不确定是否需要
	// UPROPERTY(BlueprintReadWrite, Category = "ViewFinder")
	// bool bNeedUnionToSource = false;

	UPROPERTY(BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UVFStepsRecorderWorldSubsystem> StepRecorder;
};
