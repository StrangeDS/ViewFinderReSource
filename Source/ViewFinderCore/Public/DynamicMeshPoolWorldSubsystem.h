#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "UDynamicMesh.h"

#include "DynamicMeshPoolWorldSubsystem.generated.h"

UENUM(BlueprintType)
enum class EVF_MeshType : uint8
{
	None = 0,
	Placing,
	Computing
};

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API UDynamicMeshPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UDynamicMeshPoolWorldSubsystem();

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UDynamicMesh *RequestPlacingMesh(UPrimitiveComponent *PrimitiveComponent);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void ReturnPlacingMesh(UDynamicMesh *DynamicMesh);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UDynamicMesh *RequestComputingMesh(UPrimitiveComponent *PrimitiveComponent = nullptr);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void ReturnComputingMesh(UDynamicMesh *DynamicMesh);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UDynamicMeshPool> PlacingPool;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UDynamicMeshPool> ComputingPool;
};
