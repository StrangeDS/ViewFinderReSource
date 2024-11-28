// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StepsRecordInterface.h"

#include "TransfromRecorderActor.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API ATransfromRecorderActor : public AActor, public IStepsRecordInterface
{
	GENERATED_BODY()
	
public:	
	ATransfromRecorderActor();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	// 筛选: 哪些物体需要被记录Transform. 默认实现为所有的AStaticMeshActor
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "ViewFinder")
	void ReCollectComponents();
	void ReCollectComponents_Implementation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<TObjectPtr<USceneComponent>> Components;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TMap<TObjectPtr<USceneComponent>, FTransform> TransMap;

// Implements IStepsRecordInterface:
public:
	virtual FStepInfo MakeStepInfo_Implementation() override;
	
	virtual bool StepBack_Implementation(FString &Step) override;
};
