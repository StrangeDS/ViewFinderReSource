#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"

#include "DynamicMeshPoolWorldSubsystem.h"

#include "VFDynamicMeshComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UVFDynamicMeshComponent : public UDynamicMeshComponent
{
	GENERATED_BODY()

	UVFDynamicMeshComponent(const FObjectInitializer& ObjectInitializer);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetDynamicMeshFromPool(UDynamicMesh *Mesh, EVF_MeshType Type);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	bool HasSourceComponent();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UPrimitiveComponent* GetSourceComponent();
	
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetSourceComponent(UPrimitiveComponent* PrimitiveComponent);

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	EVF_MeshType MeshType = EVF_MeshType::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UPrimitiveComponent> SourceComponent;
};
