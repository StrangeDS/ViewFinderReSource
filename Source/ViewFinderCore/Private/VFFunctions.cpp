// Fill out your copyright notice in the Description page of Project Settings.

#include "VFFunctions.h"

#include "Kismet/GameplayStatics.h"
#include "VFDynamicMeshComponent.h"
#include "VFDynamicMeshPoolWorldSubsystem.h"
#include "VFGeometryFunctions.h"

#include "Components/StaticMeshComponent.h"

#include "VFPhoto3D.h"
#include "VFViewFrustumComponent.h"
#include "VFHelperComponent.h"
#include "VFPhotoContainer.h"

bool UVFFunctions::FrustumOverlapComponents(UPrimitiveComponent *Component, const FTransform &ComponentTransform, const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes, UClass *ComponentClassFilter, const TArray<AActor *> &ActorsToIgnore, TArray<UPrimitiveComponent *> &OutComponents)
{
	OutComponents.Empty();

	if (Component == nullptr)
		return false;

	FComponentQueryParams Params(SCENE_QUERY_STAT(ComponentOverlapComponents));
	Params.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel &Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
		ObjectParams.AddObjectTypesToQuery(Channel);
	}

	const auto Wor = Component->GetWorld();
	// Component->UDynamicMeshComponent::NotifyMeshUpdated();

	bool re = Wor->ComponentOverlapMulti(Overlaps, Component, ComponentTransform.GetTranslation(), ComponentTransform.GetRotation(), Params, ObjectParams);
	UE_LOG(LogTemp, Warning, TEXT("FrustumOverlapComponents: %s"), re ? TEXT("true") : TEXT("false"));

	for (int32 OverlapIdx = 0; OverlapIdx < Overlaps.Num(); OverlapIdx++)
	{
		FOverlapResult const &O = Overlaps[OverlapIdx];
		if (O.Component.IsValid())
		{
			if (auto Act = O.GetActor())
				UE_LOG(LogTemp, Warning, TEXT("%s"), *Act->GetName());
			// if ( !ComponentClassFilter || O.Component.Get()->IsA(ComponentClassFilter) )
			// {
			// 	OutComponents.Add(O.Component.Get());
			// }
		}
	}

	return (OutComponents.Num() > 0);
}

AActor *UVFFunctions::CloneActorRuntime(AActor *Original)
{
	if (!Original || !Original->GetWorld())
		return nullptr;
	UWorld *World = Original->GetWorld();

	FActorSpawnParameters Parameters;
	Parameters.Template = Original;
	Parameters.CustomPreSpawnInitalization = [](AActor *Actor)
	{
		Actor->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	};
	AActor *Copy = World->SpawnActor<AActor>(Original->GetClass(), Parameters);
	// 手动设置transform是必要的, 否则会将Original相对于其父Actor(Photo3D)的相对Transform视为绝对Transform.
	Copy->SetActorTransform(Original->GetActorTransform());
	return Copy;
}

UVFDynamicMeshComponent *UVFFunctions::CreateVFDMComponent(AActor *Actor, UPrimitiveComponent *Parent)
{
	check(Actor);
	UWorld *World = Actor->GetWorld();
	check(World);
	UVFDynamicMeshPoolWorldSubsystem *PoolSystem = World->GetSubsystem<UVFDynamicMeshPoolWorldSubsystem>();
	check(PoolSystem);

	UVFDynamicMeshComponent *VFDMComp = NewObject<UVFDynamicMeshComponent>(Actor);
	VFDMComp->RegisterComponent();
	VFDMComp->AttachToComponent(Parent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	Actor->AddInstanceComponent(VFDMComp);
	VFDMComp->CopyMeshFromComponent(Parent);

	return VFDMComp;
}

// 对于已有VFDMComp的, 其实很简单, 它们其余的静态网格体在上次就已经被筛掉,
// 并不会参与后续的处理, 它们仅有的VFDMComp就是用于视锥处理的部分.
// 对于没有VFDMComp的, 它们第一次参与处理, 仅需处理与视锥overlap的基元组件.
// 每一个基元组件都对应一个VFDMComp(是否就挂载到对应组件上?)。
// 但并非挂载到源Actor上, 而是在新复制出来的Actor对应的基元组件上(如何映射?使用反射?).
// VFDMComp应当与对应基元组件拥有相同的响应函数(调用动态委托?功能都做到Actor接口上?).	暂使用Actor接口
TArray<UVFDynamicMeshComponent *> UVFFunctions::CheckVFDMComps(const TArray<UPrimitiveComponent *> &Components)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UVFFunctions::CheckVFDMComps()"));
	TArray<UVFDynamicMeshComponent *> Result;
	for (UPrimitiveComponent *Component : Components)
	{
		AActor *Actor = Component->GetOwner();

		auto hasVFDMComp = Actor->GetComponentByClass<UVFDynamicMeshComponent>();
		if (hasVFDMComp)
		{
			// 存在意味着处理过, 只会是VFDMComp
			auto VFDMComp = Cast<UVFDynamicMeshComponent>(Component);
			if (VFDMComp)
				Result.Add(VFDMComp);
		}
		else
		{
			// 第一次参与的Actor: 挂载VFDComp, 隐藏所有基元组件(暂只处理静态网格体).
			TArray<UPrimitiveComponent *> PrimComps;
			Actor->GetComponents<UPrimitiveComponent>(PrimComps);
			for (auto PrimComp : PrimComps)
			{
				// 只挂载被包含的组件
				if (Components.Contains(PrimComp))
				{
					// UWorld *World = Actor->GetWorld();
					// UVFDynamicMeshPoolWorldSubsystem *PoolSystem = World->GetSubsystem<UVFDynamicMeshPoolWorldSubsystem>();

					UVFDynamicMeshComponent *VFDMComp = NewObject<UVFDynamicMeshComponent>(Actor);
					VFDMComp->RegisterComponent();
					VFDMComp->AttachToComponent(PrimComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
					Actor->AddInstanceComponent(VFDMComp);
					VFDMComp->CopyMeshFromComponent(PrimComp);

					Result.Add(VFDMComp);
				}
				PrimComp->SetSimulatePhysics(false);
				PrimComp->SetCollisionProfileName("NoCollision");
				PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
				PrimComp->SetHiddenInGame(true);
			}
		}
	}
	return Result;
}

UVFDynamicMeshComponent *UVFFunctions::GetCloneVFDMComp(UVFDynamicMeshComponent *Target, AActor *Copied)
{
	TArray<UVFDynamicMeshComponent *> VFDMComps;
	Copied->GetComponents<UVFDynamicMeshComponent>(VFDMComps);
	for (const auto &VFDMComp : VFDMComps)
	{
		if (VFDMComp->GetName() == Target->GetFName())
		{
			return VFDMComp;
		}
	}
	return nullptr;
}

TArray<AActor *> UVFFunctions::CopyActorFromVFDMComps(AVFPhoto3D *Photo, const TArray<UVFDynamicMeshComponent *> &Components, TArray<UVFDynamicMeshComponent *> &CopiedComps)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UVFFunctions::CopyActorFromVFDMComps()"));
	CopiedComps.Reset();

	check(Photo);
	if (Components.Num() <= 0)
		return {};
	UWorld *World = Photo->GetWorld();

	TMap<AActor *, AActor *> ActorsMap;
	for (UVFDynamicMeshComponent *Component : Components)
	{
		AActor *Source = Component->GetOwner();
		// Actor的拷贝份, 纳入映射.
		if (!ActorsMap.Contains(Source))
		{
			AActor *Copy = CloneActorRuntime(Source);
			Copy->AttachToActor(Photo, FAttachmentTransformRules::KeepWorldTransform);
			ActorsMap.Add(Source, Copy);
		}
		AActor *Copied = ActorsMap[Source];
	}

	for (UVFDynamicMeshComponent *Component : Components)
	{
		auto CopiedComp = GetCloneVFDMComp(Component, ActorsMap[Component->GetOwner()]);
		CopiedComp->CopyMeshFromComponent(Component);
		CopiedComps.Emplace(CopiedComp);
	}

	TArray<AActor *> Result;
	for (auto &[Source, Copied] : ActorsMap)
	{
		Result.Emplace(Copied);
	}

	return Result;
}

AVFPhoto3D *UVFFunctions::TakeAPhoto(UVFViewFrustumComponent *ViewFrustum, const TArray<UPrimitiveComponent *> &Components)
{
	TRACE_CPUPROFILER_EVENT_SCOPE(TEXT("UVFFunctions::AllProccess()"));

	UWorld *World = ViewFrustum->GetWorld();
	auto VFDMComps = CheckVFDMComps(Components);
	AVFPhoto3D *Photo = World->SpawnActor<AVFPhoto3D>(ViewFrustum->GetComponentLocation(), ViewFrustum->GetComponentRotation());
	TArray<UVFDynamicMeshComponent *> CopiedComps;
	auto ActorsCopied = CopyActorFromVFDMComps(Photo, VFDMComps, CopiedComps);

	{
		check(VFDMComps.Num() == CopiedComps.Num());
		int NumOfComps = VFDMComps.Num();
		UDynamicMesh *FrustumMesh = ViewFrustum->GetDynamicMesh();
		FTransform FrustumTrans = ViewFrustum->GetComponentToWorld();

		// 原网格, 与视锥求差
		for (auto &Comp : VFDMComps)
		{
			Comp->SubtractMeshWithDMComp(ViewFrustum);
			// SubtractWithFrustum(Comp, ViewFrustum);
		}

		// 复制出的网格, 与视锥求交集
		for (auto &Comp : CopiedComps)
		{
			Comp->IntersectMeshWithDMComp(ViewFrustum);
			// IntersectWithFrustum(Comp, ViewFrustum);
		}
	}
	Photo->FoldUp();

	return Photo;
}

// void UVFFunctions::PlaceAPhoto(AVFPhoto3D *Photo, FTransform WorldTrans)
// {
// }

// AVFPhotoContainer *UVFFunctions::CreateAPhoto(UWorld *World, TSubclassOf<AVFPhotoContainer> ActorClass, FTransform WorldTrans)
// {
// 	auto Container = GameplayStatics::GetActorOfClass(World, ActorClass);
// 	check(Container);
// 	return Container->CreateAPhoto(WorldTrans);
// }

void UVFFunctions::MeshBooleanIntersect(UDynamicMeshComponent *Target, UDynamicMeshComponent *Tool)
{
	static FVF_GeometryScriptMeshBooleanOptions Options;
	UVFGeometryFunctions::ApplyMeshBoolean(
		Target->GetDynamicMesh(),
		Target->GetComponentToWorld(),
		Tool->GetDynamicMesh(),
		Tool->GetComponentToWorld(),
		EVF_GeometryScriptBooleanOperation::Intersection,
		Options);
}

void UVFFunctions::MeshBooleanSubtract(UDynamicMeshComponent *Target, UDynamicMeshComponent *Tool)
{
	static FVF_GeometryScriptMeshBooleanOptions Options;
	UVFGeometryFunctions::ApplyMeshBoolean(
		Target->GetDynamicMesh(),
		Target->GetComponentToWorld(),
		Tool->GetDynamicMesh(),
		Tool->GetComponentToWorld(),
		EVF_GeometryScriptBooleanOperation::Subtract,
		Options);
}

void UVFFunctions::IntersectWithFrustum(UVFDynamicMeshComponent *Target, UVFViewFrustumComponent *ViewFrustum)
{
	MeshBooleanIntersect(Target, ViewFrustum);
	Target->UpdateSimlpeCollision();
}

void UVFFunctions::SubtractWithFrustum(UVFDynamicMeshComponent *Target, UVFViewFrustumComponent *ViewFrustum)
{
	MeshBooleanSubtract(Target, ViewFrustum);
	Target->UpdateSimlpeCollision();
}
