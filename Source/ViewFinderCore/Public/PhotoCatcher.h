#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PhotoCatcher.generated.h"

class UStaticMeshComponent;
class UPhotoCaptureComponent;
class UViewFrustumComponent;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API APhotoCatcher : public AActor
{
	GENERATED_BODY()
	
public:	
	APhotoCatcher();

	virtual void OnConstruction(const FTransform &Transform) override;

// #if WITH_EDITOR
// 	virtual void PostEditChangeProperty(FPropertyChangedEvent& Event) override;
// #endif

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "ViewFinder")
	void TakeAPhoto();
	virtual void TakeAPhoto_Implementation();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetViewFrustumVisible(const bool &Visibility);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float ViewAngle = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float AspectRatio = 16.0f / 9;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float StartDis = 20.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float EndDis = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<TObjectPtr<AActor>> ActorsToIgnore;

	// 最好指定层级, 避免overlap到人物自身等情况.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToOverlap;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UPhotoCaptureComponent> PhotoCapture;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UViewFrustumComponent> ViewFrustum;
};
