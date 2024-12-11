#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Containers/Deque.h"
#include "VFPhotoContainer.generated.h"

class AVFPhoto2D;

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
	void AddAPhoto(AVFPhoto2D *Photo);
	
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void CreateAPhoto(const FTransform &WorldTrans);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void PrepareCurrentPhoto(float Time = -1.0f);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void GiveUpPreparing();

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
	TObjectPtr<AVFPhoto2D> CurrentPhoto2D;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder") // 不能修饰
	TDeque<AVFPhoto2D *> Photo2Ds;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	FTimerHandle PrepareTimeHandle;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<USceneComponent> Container;
};
