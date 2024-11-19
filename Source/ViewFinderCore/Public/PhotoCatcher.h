#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PhotoCatcher.generated.h"

class UTextureRenderTarget2D;

UCLASS(ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UPhotoCatcher : public USceneCaptureComponent2D
{
	GENERATED_BODY()
	
// public:
// 	UPhotoCatcher();

// 	void BeginPlay() override;

// 	void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction) override;

// 	virtual void DrawImage();

// 	virtual void CatchObjs();


// public: //From UCameraComponent
// 	virtual void OnRegister() override;
	
// 	static void AddReferencedObjects(UObject* InThis, FReferenceCollector& Collector);

// 	virtual void OnComponentDestroyed(bool bDestroyingHierarchy) override;
	
// 	void UpdateCone();
	
// protected:
// 	UTextureRenderTarget2D* Target2D;

// 	TObjectPtr<class UDrawFrustumComponent> DrawFrustum;

// 	UPROPERTY(VisibleAnywhere)
// 	UDynamicMeshComponent *Cone = nullptr;

// 	UPROPERTY(VisibleAnywhere)
// 	bool IsVideoMode = false;

// 	UPROPERTY(EditAnywhere, Category = "Demo")
// 	float ClipFar = 3000.0f;
	
// 	UPROPERTY(EditAnywhere, Category = "Demo")
// 	float AngleHor = 180;
	
// 	UPROPERTY(EditAnywhere, Category = "Demo")
// 	float AngleVer = 100;
	
// 	UPROPERTY(EditAnywhere, Category = "Demo")
// 	int Width = 1024;

// 	UPROPERTY(EditAnywhere, Category = "Demo")
// 	int Height = 1024;
	
};
