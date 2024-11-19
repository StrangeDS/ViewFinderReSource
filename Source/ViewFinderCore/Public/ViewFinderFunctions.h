// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"

// #include "Engine/CollisionProfile.h"
// #include "CollisionQueryParams.h"
// #include "Engine/OverlapResult.h"


#include "Components/MeshComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/BaseDynamicMeshComponent.h"

#include "ViewFinderFunctions.generated.h"

UCLASS(meta = (ScriptName = "GeometryScript_Primitives|ViewFinder"))
class VIEWFINDERCORE_API UViewFinderFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<FVector> CalculateViewFrustumPostions(float Angle, float AspectRatio, float StartDis, float EndDis);

	// UFUNCTION(BlueprintCallable, Category = "GeometryScript|Primitives|ViewFinder", meta=(ScriptMethod))
	// static UPARAM(DisplayName = "Target Mesh") UDynamicMesh* 
	// AppendViewFrustum( 
	// 	UDynamicMesh* TargetMesh, 
	// 	FGeometryScriptPrimitiveOptions PrimitiveOptions,
	// 	FTransform Transform,
	// 	float AspectAngle = 90,
	// 	float AspectRatio = 1.777778f,
	// 	float StartDis = 10,
	// 	float EndDis = 1000,
	// 	EGeometryScriptPrimitiveOriginMode Origin = EGeometryScriptPrimitiveOriginMode::Base,
	// 	UGeometryScriptDebug* Debug = nullptr);


	// copy from ComponentOverlapComponents() in KismetSystemLibrary(.h). 
	UFUNCTION(BlueprintCallable, Category = "ViewFinder", meta=(AutoCreateRefTerm="ActorsToIgnore", DisplayName="Frustum Overlap Components"))
	static bool FrustumOverlapComponents(UPrimitiveComponent* Component, const FTransform& ComponentTransform, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<class UPrimitiveComponent*>& OutComponents);
};
