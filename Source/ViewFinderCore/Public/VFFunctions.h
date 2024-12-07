// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "Components/MeshComponent.h"
#include "Components/DynamicMeshComponent.h"
#include "Components/BaseDynamicMeshComponent.h"

#include "VFFunctions.generated.h"

UCLASS(meta = (ScriptName = "ViewFinder"))
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
	static TArray<AActor *> CopyActorFromVFDMComps(AVFPhoto3D *Photo, const TArray<UVFDynamicMeshComponent *> &Components, TArray<UVFDynamicMeshComponent *> &CopiedComps);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void FilterPrimCompsWithHelper(TArray<UPrimitiveComponent *> &Components);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static AVFPhoto3D *TakeAPhoto(UVFViewFrustumComponent *ViewFrustum, const TArray<UPrimitiveComponent *> &Components);
	
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void PlaceAPhoto(AVFPhoto3D *Photo);
};
