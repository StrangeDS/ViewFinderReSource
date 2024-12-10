#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Deque.h"
#include "VFPhotoContainer.generated.h"

class AVFPhoto3D;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API AVFPhotoContainer : public AActor
{
	GENERATED_BODY()

public:
	AVFPhotoContainer();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void AddAPhoto(AVFPhoto3D *Photo);
	
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void CreateAPhoto(const FTransform &WorldTrans);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void PrepareCurrentPhoto(float Time = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void GiveUpPrepare();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void PlaceCurrentPhoto();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void ChangeCurrentPhoto(const bool &Next);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void UpdateCurrentPhoto();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	bool bFocusOn = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float TimeOfSelect = 0.5f;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<AVFPhoto3D> CurrentPhoto;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder") // 不能修饰
	TDeque<AVFPhoto3D *> Photos;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	FTimerHandle PrepareTimeHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<USceneComponent> Container;
};
