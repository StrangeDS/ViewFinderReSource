#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VF_InteractInterface.h"

#include "PhotoCatcher.generated.h"

class UStaticMeshComponent;
class UPhotoCaptureComponent;
class UViewFrustumComponent;
class UUserWidget;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API APhotoCatcher : public AActor, public IVF_InteractInterface
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

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float ViewAngle = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float AspectRatio = 16.0f / 9;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float StartDis = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	float EndDis = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UPhotoCaptureComponent> PhotoCapture;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UViewFrustumComponent> ViewFrustum;

protected:	// IVF_InteractInterface needs
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TSubclassOf<UUserWidget> HintUMGClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UUserWidget> HintUMG;

public:		// implements IVF_InteractInterface
	virtual bool StartAiming_Implementation(APlayerController* Controller);

	virtual bool EndAiming_Implementation(APlayerController* Controller);

	virtual bool Interact_Implementation(APlayerController* Controller);
};
