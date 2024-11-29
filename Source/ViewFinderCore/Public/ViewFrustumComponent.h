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
	FGeometryScriptPrimitiveOptions PrimitiveOptions{
		EGeometryScriptPrimitivePolygroupMode::SingleGroup};

	// 重点在于: 凸包生成, 面数, 不要识别为基础形状.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	FGeometryScriptCollisionFromMeshOptions CollisionOptions{
		false,
		EGeometryScriptCollisionGenerationMethod::ConvexHulls,
		false,
		false,
		false,
		1.0,
		true,
		6};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UMaterialInterface> Matirial;
};
