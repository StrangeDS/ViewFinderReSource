#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"

#include "UDynamicMesh.h"

#include "DynamicMeshPoolWorldSubsystem.generated.h"

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API UDynamicMeshPoolWorldSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

	UDynamicMeshPoolWorldSubsystem();

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UDynamicMesh* RequestPlacingMesh(UPrimitiveComponent* PrimitiveComponent);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	UDynamicMesh* RequestComputingMesh(UPrimitiveComponent* PrimitiveComponent = nullptr);
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	UDynamicMeshPool* PlacingPool;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	UDynamicMeshPool* ComputingPool;
};
