#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "ViewFrustumComponent.generated.h"

// struct FGeometryScriptPrimitiveOptions;
// struct FGeometryScriptCollisionFromMeshOptions;

/**
 * By StrangeDS
 */
UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UViewFrustumComponent : public UDynamicMeshComponent
{
	GENERATED_BODY()

	UViewFrustumComponent();
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GenerateViewFrustum();
	void GenerateViewFrustum_Implementation();

	// 需要在Actor的construction中调用
	UFUNCTION(BlueprintCallable)
	void RegenerateViewFrustum();

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder", meta = (ClampMin = "0.01", ClampMax = "359.99"))
	float FieldOfView = 90.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder", meta = (ClampMin = "0.1", ClampMax = "10.0"))
	float AspectRatio = 16.0f / 9;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float StartDis = 10.0f;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	float EndDis = 100.0f;

	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	// FGeometryScriptPrimitiveOptions PrimitiveOptions;
	
	// UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	// FGeometryScriptCollisionFromMeshOptions CollisionOptions;
};
