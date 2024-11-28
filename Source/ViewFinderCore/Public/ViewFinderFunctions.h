// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"

#include "Components/MeshComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/BaseDynamicMeshComponent.h"

#include "ViewFinderFunctions.generated.h"

UCLASS(meta = (ScriptName = "ViewFinder"))
class VIEWFINDERCORE_API UViewFinderFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<FVector> CalculateViewFrustumPositions(float Angle, float AspectRatio, float StartDis, float EndDis);

	// copy from ComponentOverlapComponents() in KismetSystemLibrary(.h).
	UFUNCTION(BlueprintCallable, Category = "ViewFinder", meta = (AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Frustum Overlap Components"))
	static bool FrustumOverlapComponents(UPrimitiveComponent *Component, const FTransform &ComponentTransform, const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes, UClass *ComponentClassFilter, const TArray<AActor *> &ActorsToIgnore, TArray<class UPrimitiveComponent *> &OutComponents);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static AActor *CloneActorRuntime_Abort(AActor *Original);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static AActor *CloneActorRuntime(AActor *Original);

	/// @brief 默认的VFDMComponent复制函数, 包括碰撞, 使用对象池.
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static UVFDynamicMeshComponent *CreateVFDMComponent(UObject *Outer, UPrimitiveComponent *Template = nullptr);

	/// @brief 检验未处理过的的Actor, 处理为: 将其相关基元替换为VFDMComp, 关闭隐藏其它基元组件.
	/// @param OverlapsComps
	/// @return (复制)替换后的, 全为VFDMComp的列表.
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<UVFDynamicMeshComponent *> CheckVFDMComps(const TArray<UPrimitiveComponent *> &Components);

	/// @brief 复制VFDMComp列表的Actor
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<AActor *> CopyActorFromVFDMComps(const TArray<UVFDynamicMeshComponent *> &Components);

	// UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	// static void (const TArray<AActor *> &Actors);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<AActor *> AllProccess(const TArray<UPrimitiveComponent *> &Components);
};
