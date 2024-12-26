#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "VFHelperComponent.h"
#include "VFPawnStandIn.h"

#include "VFFunctions.generated.h"

UCLASS(meta = (ScriptName = "VFFunctions"))
class VIEWFINDERCORE_API UVFFunctions : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static AActor *CloneActorRuntime(AActor *Original);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static bool CheckPawnComps(
		TArray<UPrimitiveComponent *> &Components,
		TSubclassOf<AVFPawnStandIn> PawnStandInClass,
		bool NeedStandIn = false);

	/// @brief 检验未处理过的的Actor, 处理为: 将其相关基元替换为VFDMComp, 关闭隐藏其它基元组件.
	/// @param OverlapsComps
	/// @return (复制)替换后的, 全为VFDMComp的列表.
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<UVFDynamicMeshComponent *> CheckVFDMComps(
		const TArray<UPrimitiveComponent *> &Components,
		TSubclassOf<UVFDynamicMeshComponent> VFDMCompClass);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static UVFDynamicMeshComponent *GetCloneVFDMComp(UVFDynamicMeshComponent *Target, AActor *Copied);

	/// @brief 复制VFDMComp列表的Actor
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static TArray<AActor *> CopyActorFromVFDMComps(
		UWorld* World,
		UPARAM(ref) const TArray<UVFDynamicMeshComponent *> &Components,
		UPARAM(ref) TArray<UVFDynamicMeshComponent *> &CopiedComps);

	template <typename T>
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	static void GetCompsToHelpersMapping(
		UPARAM(ref) TArray<T *> &Components,
		UPARAM(ref) TMap<UPrimitiveComponent *, UVFHelperComponent *> &Map);
};

template <typename T>
inline void UVFFunctions::GetCompsToHelpersMapping(UPARAM(ref) TArray<T *> &Components, UPARAM(ref) TMap<UPrimitiveComponent *, UVFHelperComponent *> &Map)
{
	check(T::StaticClass()->IsChildOf(UPrimitiveComponent::StaticClass()));
	for (auto It = Components.CreateIterator(); It; It++)
	{
		auto Helper = (*It)->GetOwner()->GetComponentByClass<UVFHelperComponent>();
		if (Helper)
			Map.Add(Cast<UPrimitiveComponent>(*It), Helper);
	}
}
