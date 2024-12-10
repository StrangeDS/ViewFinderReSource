// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Components/MeshComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/BaseDynamicMeshComponent.h"
#include "VFHelperComponent.h"

#include "VFFunctions.generated.h"

UCLASS(meta = (ScriptName = "VFFunctions"))
class VIEWFINDERCORE_API UVFFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Need? from ComponentOverlapComponents() in KismetSystemLibrary(.h).
	UFUNCTION(BlueprintCallable, Category = "ViewFinder", meta = (AutoCreateRefTerm = "ActorsToIgnore", DisplayName = "Frustum Overlap Components"))
	static bool FrustumOverlapComponents(UPrimitiveComponent *Component, const FTransform &ComponentTransform, const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes, UClass *ComponentClassFilter, const TArray<AActor *> &ActorsToIgnore, TArray<class UPrimitiveComponent *> &OutComponents);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static AActor *CloneActorRuntime(AActor *Original);

	/// @brief 默认的VFDMComponent复制函数, 包括碰撞, 使用对象池.
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static UVFDynamicMeshComponent *CreateVFDMComponent(AActor *Actor, UPrimitiveComponent *Parent = nullptr);

	/// @brief 检验未处理过的的Actor, 处理为: 将其相关基元替换为VFDMComp, 关闭隐藏其它基元组件.
	/// @param OverlapsComps
	/// @return (复制)替换后的, 全为VFDMComp的列表.
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<UVFDynamicMeshComponent *> CheckVFDMComps(const TArray<UPrimitiveComponent *> &Components);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static UVFDynamicMeshComponent *GetCloneVFDMComp(UVFDynamicMeshComponent *Target, AActor *Copied);

	/// @brief 复制VFDMComp列表的Actor
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<AActor *> CopyActorFromVFDMComps(
		AVFPhoto3D *Photo,
		UPARAM(ref) const TArray<UVFDynamicMeshComponent *> &Components,
		UPARAM(ref) TArray<UVFDynamicMeshComponent *> &CopiedComps);

	// UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	// static void FilterPrimCompsWithHelper(
	// 	TArray<UPrimitiveComponent *> &Components,
	// 	TMap<UVFDynamicMeshComponent *, UVFHelperComponent *> &Map);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void GetCompsToHelpersMapping(
		UPARAM(ref) TArray<UVFDynamicMeshComponent *> &Components,
		UPARAM(ref) TMap<UVFDynamicMeshComponent *, UVFHelperComponent *> &Map);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static AVFPhoto3D *TakeAPhoto(
		UVFViewFrustumComponent *ViewFrustum,
		UPARAM(ref) const TArray<UPrimitiveComponent *> &Components);

	// UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	// static void PlaceAPhoto(AVFPhoto3D *Photo, FTransform WorldTrans);.

	// UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	// static AVFPhotoContainer *CreateAPhoto(UWorld *World, FTransform WorldTrans);

public: // 网格布尔操作
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void MeshBooleanIntersect(UDynamicMeshComponent *Target, UDynamicMeshComponent *Tool);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void MeshBooleanSubtract(UDynamicMeshComponent *Target, UDynamicMeshComponent *Tool);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void IntersectWithFrustum(UVFDynamicMeshComponent *Target, UVFViewFrustumComponent *ViewFrustum);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void SubtractWithFrustum(UVFDynamicMeshComponent *Target, UVFViewFrustumComponent *ViewFrustum);
};
