#pragma once

#include "CoreMinimal.h"
#include "VFPhotoCatcher_Interact.h"
#include "VFPhotoCatcher_PickUp.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERDEMO_API AVFPhotoCatcher_PickUp : public AVFPhotoCatcher_Interact
{
	GENERATED_BODY()

public:
	virtual bool Interact_Implementation(APlayerController* Controller) override;

	virtual void TakeAPhoto_Implementation() override;
	
	virtual void CloseToPreview_Implementation() override;
	
	virtual void LeaveFromPreview_Implementation() override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	void PickUp(USceneComponent* ToAttach);
	void PickUp_Implementation(USceneComponent* ToAttach);

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	void DropDown();
	void DropDown_Implementation();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	class UInputMappingContext *HoldingMappingContext;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	bool bPickedUp = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	bool bReady = false;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	FTimerHandle PreviewTimeHanlde;
};
