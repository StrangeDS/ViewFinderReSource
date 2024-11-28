#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"
#include "VFDynamicMeshComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UVFDynamicMeshComponent : public UDynamicMeshComponent
{
	GENERATED_BODY()

	UVFDynamicMeshComponent(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	bool HasSourceComponent();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UPrimitiveComponent* GetSourceComponent();
	
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetSourceComponent(UPrimitiveComponent* PrimitiveComponent);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UPrimitiveComponent> SourceComponent;
};
