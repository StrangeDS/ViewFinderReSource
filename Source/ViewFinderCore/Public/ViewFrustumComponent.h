#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"

#include "GeometryScript/MeshPrimitiveFunctions.h"
#include "GeometryScript/CollisionFunctions.h"

#include "ViewFrustumComponent.generated.h"


UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UViewFrustumComponent : public UDynamicMeshComponent
{
	GENERATED_BODY()

	UViewFrustumComponent();
	
public:
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void GenerateViewFrustum(float Angle, float AspectRatio, float StartDis, float EndDis);
	virtual void GenerateViewFrustum_Implementation(float Angle, float AspectRatio, float StartDis, float EndDis);

	// 需要在Actor的construction中调用
	UFUNCTION(BlueprintCallable)
	void RegenerateViewFrustum(float Angle = 90.f, float AspectRatio = 1.77778f, float StartDis = 10.0f, float EndDis = 500.0f);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
    FGeometryScriptPrimitiveOptions PrimitiveOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
    FGeometryScriptCollisionFromMeshOptions CollisionOptions;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UMaterialInterface> Matirial;
};
