#pragma once

#include "CoreMinimal.h"
#include "Components/DynamicMeshComponent.h"

#include "VFDynamicMeshPoolWorldSubsystem.h"

#include "VFDynamicMeshComponent.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UVFDynamicMeshComponent : public UDynamicMeshComponent
{
	GENERATED_BODY()

public:
	UVFDynamicMeshComponent(const FObjectInitializer& ObjectInitializer);

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetDynamicMeshFromPool(UDynamicMesh *Mesh, EVFMeshType Type);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	virtual void CopyMeshFromComponent(UPrimitiveComponent* Source);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void UpdateSimlpeCollision();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetEnabled(bool Enabled);

public:	// SourceComponent
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UPrimitiveComponent* GetSourceComponent();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	bool IsSourceVFDMComp();
	
protected:	// Enabled
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	bool bEnabled = true;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	bool bSimulatePhysicsRecorder = false;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	bool bEnableGravityRecorder = false;

protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadWrite, Category = "ViewFinder")
	EVFMeshType MeshType = EVFMeshType::None;

	UPROPERTY(BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UPrimitiveComponent> SourceComponent;
};
