#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"

#include "VFDynamicMeshPoolWorldSubsystem.h"

#include "VFDynamicMeshComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UVFDynamicMeshComponent : public UDynamicMeshComponent
{
	GENERATED_BODY()

	UVFDynamicMeshComponent(const FObjectInitializer& ObjectInitializer);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetDynamicMeshFromPool(UDynamicMesh *Mesh, EVFMeshType Type);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	virtual void CopyMeshFromComponent(UPrimitiveComponent* Source);

	void UpdateSimlpeCollision();

public:	// SourceComponent
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UPrimitiveComponent* GetSourceComponent();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	bool IsSourceVFDMComp();

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	EVFMeshType MeshType = EVFMeshType::None;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UPrimitiveComponent> SourceComponent;
};
