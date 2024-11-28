// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewFinderFunctions.h"

#include "Kismet/GameplayStatics.h"
#include "VFDynamicMeshComponent.h"
#include "DynamicMeshPoolWorldSubsystem.h"

#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/SceneUtilityFunctions.h"
#include "GeometryScript/CollisionFunctions.h"

#include "Components/StaticMeshComponent.h"

#include "PhotoActor.h"

TArray<FVector> UViewFinderFunctions::CalculateViewFrustumPositions(
	float Angle,
	float AspectRatio,
	float StartDis,
	float EndDis)
{
	TArray<FVector> Res;
	Res.Reserve(8);
	auto CalculatePositions = [&](float Distance)
	{
		float x = Distance;
		float y = Distance * tanf(FMath::DegreesToRadians(Angle) / 2);
		float z = y / AspectRatio;
		Res.Push({x, -y, -z});
		Res.Push({x, y, -z});
		Res.Push({x, y, z});
		Res.Push({x, -y, z});
	};
	CalculatePositions(StartDis);
	CalculatePositions(EndDis);
	return Res;
}

bool UViewFinderFunctions::FrustumOverlapComponents(UPrimitiveComponent *Component, const FTransform &ComponentTransform, const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes, UClass *ComponentClassFilter, const TArray<AActor *> &ActorsToIgnore, TArray<UPrimitiveComponent *> &OutComponents)
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

	// // New version that's more generic and relies on the physics object interface.
	// TArray<Chaos::FPhysicsObjectHandle> PhysicsObjects = Component->GetAllPhysicsObjects();
	// FLockedReadPhysicsObjectExternalInterface Interface = FPhysicsObjectExternalInterface::LockRead(PhysicsObjects);

	// // For geometry collections we want to make sure we don't account for disabled particles since that means their parent cluster still hasn't released them yet.
	// // This should be safe for non-geometry collections and not filter out anything.
	// PhysicsObjects = PhysicsObjects.FilterByPredicate(
	// 	[&Interface](Chaos::FPhysicsObject* Object)
	// 	{
	// 		return !Interface->AreAllDisabled({ &Object, 1 });
	// 	}
	// );

	// if (PhysicsObjects.IsEmpty())
	// {
	// 	UE_LOG(LogCollision, Log, TEXT("UWorld::ComponentOverlapMultiByChannel : (%s) No physics data"), *Component->GetName());
	// 	return false;
	// }

	// bool bHaveBlockingHit = false;
	// FCollisionResponseParams CollisionResponseParams{ Component->GetCollisionResponseToChannels() };

	// FComponentQueryParams ParamsWithSelf = Params;
	// ParamsWithSelf.AddIgnoredComponent_LikelyDuplicatedRoot(Component);
	// const auto Quat = ComponentTransform.Rotator();
	// const auto Pos = ComponentTransform.GetLocation();
	// const FTransform ComponentToTest{ Quat, Pos };
	// const FTransform WorldToComponent = Component->GetComponentToWorld().Inverse();
	// const auto TraceChannel = Component->GetCollisionObjectType();

	// TArray<struct FOverlapResult> TempOverlaps;
	// for (Chaos::FPhysicsObjectHandle Object : PhysicsObjects)
	// {
	// 	TArray<Chaos::FShapeInstanceProxy*> Shapes = Interface->GetAllThreadShapes({&Object, 1});

	// 	// Determine how to convert the local space of this body instance to the test space
	// 	const FTransform ObjectToWorld = Interface->GetTransform(Object);
	// 	const FTransform ObjectToTest = ComponentToTest * WorldToComponent * ObjectToWorld;

	// 	for (Chaos::FShapeInstanceProxy* Shape : Shapes)
	// 	{
	// 		FPhysicsShapeHandle ShapeRef{ Shape, nullptr };

	// 		// TODO: Add support to be able to check if the shape collision is enabled for the generic physics object interface.
	// 		/*
	// 		// Skip this shape if it's CollisionEnabled setting was masked out
	// 		if (ParamsWithSelf.ShapeCollisionMask && !(ParamsWithSelf.ShapeCollisionMask & BodyInstance->GetShapeCollisionEnabled(ShapeIdx)))
	// 		{
	// 			continue;
	// 		}
	// 		*/
	// 		FPhysicsGeometryCollection GeomCollection = FPhysicsInterface::GetGeometryCollection(ShapeRef);
	// 		if (!ShapeRef.GetGeometry().IsConvex())
	// 		{
	// 			continue;	//we skip complex shapes - should this respect ComplexAsSimple?
	// 		}

	// 		TempOverlaps.Reset();
	// 		if (FPhysicsInterface::GeomOverlapMulti(Wor, GeomCollection, ObjectToTest.GetTranslation(), ObjectToTest.GetRotation(), TempOverlaps, TraceChannel, ParamsWithSelf, CollisionResponseParams, ObjectParams))
	// 		{
	// 			bHaveBlockingHit = true;
	// 		}
	// 		Overlaps.Append(TempOverlaps);
	// 	}
	// }

	// if (!bHaveBlockingHit)
	// {
	// 	UE_LOG(LogCollision, Log, TEXT("%s bHaveBlockingHit is false"), *Component->GetName());
	// 	return false;
	// }
	// 	//

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

// from https://forums.unrealengine.com/t/cloning-actors-in-runtime/768883/5
AActor *UViewFinderFunctions::CloneActorRuntime_Abort(AActor *Original)
{
	return nullptr;

	// AActor *Copy = UGameplayStatics::BeginDeferredActorSpawnFromClass(Original->GetWorld(), Original->GetClass(), Original->GetTransform());
	// 	// CopyPropertiesForUnrelatedObjects(Original, Copy, FCopyPropertiesForUnrelatedObjectsParams());
	// 	Copy->CopyRuntimeProperties(Original);

	// 	TArray<UActorComponent*> Components;
	// Original->GetComponents(Components);
	// 	for (auto Component : Components)
	// 	{
	// 		UActorComponent* NewComponent = Copy->AddComponent(Component->GetClass());
	// 		// CopyPropertiesForUnrelatedObjects(Component, NewComponent);
	// 		NewComponent->CopyPropertiesFromComponent(Component);
	// 	}

	// 	UGameplayStatics::FinishSpawningActor(Copy, Original->GetTransform());

	// 	return Copy;
}

AActor *UViewFinderFunctions::CloneActorRuntime(AActor *Original)
{
	if (!Original || !Original->GetWorld())
		return nullptr;
	UWorld *World = Original->GetWorld();

	FActorSpawnParameters Parameters;
	// Parameters.Name = Original->GetFName().AppendString(FString("*"));
	Parameters.Template = Original;
	// Parameters.Owner = ;
	AActor *Copy = World->SpawnActor<AActor>(Original->GetClass(), Parameters);
	
	// 一般情况下我们并不关心动态添加的组件. 但果有需求, 也可以将它们复制出来, 如下:
	// UVFDynamicMeshComponent的复制需要额外处理，故在此不关心
	// TArray<TSubclassOf<UObject>> Unconcerned = { TSubclassOf<UVFDynamicMeshComponent>() };
	// TArray<UActorComponent*> Components = Original->GetInstanceComponents();
	// for (auto Component : Components)
	// {
	// 	// 筛选不关心的动态组件(如UVFDynamicMeshComponent).
	// 	bool NeedToCopy = true;
	// 	UClass* ClassType = Component->GetClass();
	// 	for (auto Type : Unconcerned) {
	// 		if (ClassType->IsChildOf(Type)) {
	// 			NeedToCopy = false;
	// 			break;
	// 		}
	// 	}
	// 	if (!NeedToCopy) continue;

	// 	UActorComponent* Comp = NewObject<UActorComponent>(
	// 		Copy,
	// 		Component->GetFName(),
	// 		EObjectFlags::RF_NoFlags,
	// 		Component
	// 		);
	// 	Comp->RegisterComponent();
	// 	Copy->AddInstanceComponent(Comp);
	// 	if (auto SceneComponent = Cast<USceneComponent>(Comp)) {
	// 		SceneComponent->AttachToComponent(Copy->GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	// 	}
	// }
	return Copy;
}

UVFDynamicMeshComponent *UViewFinderFunctions::CreateVFDMComponent(UObject *Outer, UPrimitiveComponent *Template)
{
	check(Outer);
	UWorld *World = Outer->GetWorld();
	check(World);
	UDynamicMeshPoolWorldSubsystem *PoolSystem = World->GetSubsystem<UDynamicMeshPoolWorldSubsystem>();
	check(PoolSystem);

	UVFDynamicMeshComponent *VFDMComp = NewObject<UVFDynamicMeshComponent>(Outer);
	if (Template)
	{
		// 复制网格
		UDynamicMesh *DynamicMesh = PoolSystem->RequestPlacingMesh(Template);
		VFDMComp->SetDynamicMesh(DynamicMesh);

		// 复制碰撞预设, 类型等. 原静态网格体的简单碰撞可能与显示不一致, 自动生成的碰撞并不完美(以及不能智能地选择生成碰撞的设置).
		VFDMComp->SetCollisionProfileName(Template->GetCollisionProfileName());
		VFDMComp->SetComplexAsSimpleCollisionEnabled(true);
		FGeometryScriptCollisionFromMeshOptions Options;
		Options.Method = EGeometryScriptCollisionGenerationMethod::AlignedBoxes;
		UGeometryScriptLibrary_CollisionFunctions::SetDynamicMeshCollisionFromMesh(
			DynamicMesh,
			VFDMComp,
			Options,
			nullptr);
		VFDMComp->SetCollisionEnabled(Template->GetCollisionEnabled());

		// 复制物理
		VFDMComp->SetEnableGravity(Template->IsGravityEnabled()); 
		VFDMComp->SetSimulatePhysics(Template->BodyInstance.bSimulatePhysics);
		
		// 复制材质
		VFDMComp->SetMaterial(0, Template->GetMaterial(0));
		// TODO: 传递事件. 暂使用Actor接口
	}

	return VFDMComp;
}

// 对于已有VFDMComp的, 其实很简单, 它们其余的静态网格体在上次就已经被筛掉,
// 并不会参与后续的处理, 它们仅有的VFDMComp就是用于视锥处理的部分.
// 对于没有VFDMComp的, 它们第一次参与处理, 仅需处理与视锥overlap的基元组件.
// 每一个基元组件都对应一个VFDMComp(是否就挂载到对应组件上?)。
// 但并非挂载到源Actor上, 而是在新复制出来的Actor对应的基元组件上(如何映射?使用反射?).
// VFDMComp应当与对应基元组件拥有相同的响应函数(调用动态委托?功能都做到Actor接口上?).	暂使用Actor接口
TArray<UVFDynamicMeshComponent *> UViewFinderFunctions::CheckVFDMComps(const TArray<UPrimitiveComponent *> &Components)
{
	TArray<UVFDynamicMeshComponent *> Result;
	for (UPrimitiveComponent *Component : Components)
	{
		auto hasVFDMComp = Component->GetOwner()->GetComponentByClass<UVFDynamicMeshComponent>();
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
			AActor *Actor = Component->GetOwner();
			TArray<UPrimitiveComponent *> PrimComps;
			Actor->GetComponents<UPrimitiveComponent>(PrimComps);
			for (auto PrimComp : PrimComps)
			{
				// 只挂载被包含的组件
				if (Components.Contains(PrimComp))
				{
					// 不确定的: 额外处理VFDMComp被SMComp挤开的特殊情况
					bool bPhysics = PrimComp->BodyInstance.bSimulatePhysics;
					PrimComp->SetSimulatePhysics(false);

					UVFDynamicMeshComponent *VFDMComp = CreateVFDMComponent(Actor, PrimComp);
					VFDMComp->RegisterComponent();
					VFDMComp->AttachToComponent(PrimComp, FAttachmentTransformRules::KeepWorldTransform);
					Actor->AddInstanceComponent(VFDMComp);

					VFDMComp->SetSimulatePhysics(bPhysics);
					Result.Add(VFDMComp);
				}
				PrimComp->SetHiddenInGame(true);
				PrimComp->SetSimulatePhysics(false);
				PrimComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
			}
		}
	}
	return Result;
}

TArray<AActor *> UViewFinderFunctions::CopyActorFromVFDMComps(const TArray<UVFDynamicMeshComponent *> &Components)
{
	UWorld* World = Components[0]->GetWorld();
	if (!World) return {};

	APhotoActor* Photo = World->SpawnActor<APhotoActor>();

	TMap<AActor *, AActor *> ActorsMap;
	for (UVFDynamicMeshComponent *Component : Components)
	{
		AActor *Source = Component->GetOwner();
		// Actor的拷贝份, 纳入映射.
		if (!ActorsMap.Contains(Source))
		{
			AActor *Copy = CloneActorRuntime(Source);
			// UVFDynamicMeshComponent会被复制出来, 但Mesh并不会复制, 需要手动设置Mesh.
			Copy->AttachToActor(Photo, FAttachmentTransformRules::KeepWorldTransform);

			ActorsMap.Add(Source, Copy);
		}
		AActor *Copied = ActorsMap[Source];

		// auto SourceCompName = Component->GetAttachParent()->GetFName();
		// auto AttachedParent = Cast<UPrimitiveComponent>(Copied->GetDefaultSubobjectByName(SourceCompName)); // 通过反射, 不支持动态添加的组件
		
		// auto CompName = Component->GetFName();
		// auto CompCopied = Copied->GetDefaultSubobjectByName(CompName);
		// if (!CompCopied) UE_LOG(LogTemp, Warning, TEXT("Copied gets the relative DMcomponent failed."));
		// check(CompCopied);
		// auto VFDMCompCopied = Cast<UVFDynamicMeshComponent>(CompCopied);
		// auto VFDMCompCopied = Cast<UVFDynamicMeshComponent>(Copied->GetDefaultSubobjectByName(CompName)); // 通过反射, 不支持动态添加的组件
		// auto VFDMCompCopied = FindObject<UVFDynamicMeshComponent>(Copied, CompName, true);				// 名称逐层查找, 支持动态添加的组件
		// Actor::GetComponentsByClass();
		// check(VFDMCompCopied);
		// UVFDynamicMeshComponent::CopyMeshes(Component, VFDMCompCopied);

		// UVFDynamicMeshComponent *CopiedVFDMComp = CreateVFDMComponent(Copied, Component);
		// CopiedVFDMComp->SetSourceComponent(Component);

		// FName SourceCompName = Component->GetAttachParent()->GetFName();
		// auto ToAttach = Cast<UPrimitiveComponent>(Copied->GetDefaultSubobjectByName(SourceCompName)); // 通过反射, 不支持动态添加的组件
		// // auto ToAttach = FindObject<UPrimitiveComponent>(Copied, CompName, true):				// 名称逐层查找, 支持动态添加的组件
		// check(ToAttach);
		// CopiedVFDMComp->RegisterComponent();
		// CopiedVFDMComp->AttachToComponent(ToAttach, FAttachmentTransformRules::KeepRelativeTransform);
		// Copied->AddInstanceComponent(CopiedVFDMComp);
	}

	TArray<AActor *> Result;
	for (auto &[Source, Copied] : ActorsMap)
	{
		Result.Emplace(Copied);
	}
	return Result;
}

TArray<AActor *> UViewFinderFunctions::AllProccess(const TArray<UPrimitiveComponent *> &Components)
{
	auto VFDMComps = CheckVFDMComps(Components);
	auto ActorsCopied = CopyActorFromVFDMComps(VFDMComps);
	// for (Actor : ActorsCopied) {}

    return TArray<AActor *>();
}
