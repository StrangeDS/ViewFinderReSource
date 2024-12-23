#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "VFStepsRecordInterface.h"

#include "VFStepsRecorderWorldSubsystem.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVFStepRecorderDelegate, float, Time);

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API UVFStepsRecorderWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()

public:
	virtual TStatId GetStatId() const override;
	
	virtual void Initialize(FSubsystemCollectionBase& Collection) override;
	virtual void Deinitialize() override;
	virtual void OnWorldBeginPlay(UWorld& InWorld) override;
	virtual void Tick(float DeltaTime) override;

public:
	FORCEINLINE float GetTime() { return Time; }
	
	FORCEINLINE float GetDeltaTime() { return TickInterval; }

	// 无需tick的可以在此提交
	void SubmitStep(UObject *Sender, FVFStepInfo &Info);

	// 未注册的瞬时Step
	TArray<FVFStepInfo> Infos;

public:
	void TickForward(float DeltaTime);

	void TickBackward(float DeltaTime);
	
	UPROPERTY(BlueprintAssignable, Category = "ViewFinder")
	FVFStepRecorderDelegate OnTickTime;

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void RegisterTickable(TScriptInterface<IVFStepsRecordInterface> Target);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void UnregisterTickable(TScriptInterface<IVFStepsRecordInterface> Target);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TArray<TScriptInterface<IVFStepsRecordInterface>> TickTargets;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void StartRewinding();
	
	UPROPERTY(BlueprintAssignable, Category = "ViewFinder")
	FVFStepRecorderDelegate OnStartRewinding;

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void EndRewinding();

	UPROPERTY(BlueprintAssignable, Category = "ViewFinder")
	FVFStepRecorderDelegate OnEndRewinding;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	bool bIsRewinding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float RewindFactor = 3.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float RewindCurFactor = RewindFactor;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float Time = TIME_MIN;

	// 自定义tick时间
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float TickInterval = 0.05f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float TimeSinceLastTick = 0.f;
	
    const static float TIME_MAX;
    const static float TIME_MIN;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void RewindToLastKey();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float TimeOfRewindToLastKey = 3.0f;

	FTimerHandle RewindHandle;
};
