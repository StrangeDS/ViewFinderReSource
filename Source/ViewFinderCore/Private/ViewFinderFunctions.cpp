// Fill out your copyright notice in the Description page of Project Settings.


#include "ViewFinderFunctions.h"



// #include "UDynamicMesh.h"

// #include "Generators/MeshShapeGenerator.h"
// #include "Generators/GridBoxMeshGenerator.h"

// extern static void AppendPrimitive(
// 	UDynamicMesh* TargetMesh, 
// 	FMeshShapeGenerator* Generator, 
// 	FTransform Transform, 
// 	FGeometryScriptPrimitiveOptions PrimitiveOptions,
// 	FVector3d PreTranslate = FVector3d::Zero());

TArray<FVector> UViewFinderFunctions::CalculateViewFrustumPostions(
	float Angle,
	float AspectRatio,
	float StartDis,
	float EndDis)
{
	TArray<FVector> Res;
	Res.Reserve(8);
	float x = 0.f, y = 0.f, z = 0.f;
	{
		x = StartDis;
		y = StartDis * tanf(FMath::DegreesToRadians(Angle) / 2);
		z = y / AspectRatio;
		Res.Push({x, -y, -z});
		Res.Push({x, y, -z});
		Res.Push({x, y, z});
		Res.Push({x, -y, z});
	}
	{
		x = EndDis;
		y = EndDis * tanf(FMath::DegreesToRadians(Angle) / 2);
		z = y / AspectRatio;
		Res.Push({x, -y, -z});
		Res.Push({x, y, -z});
		Res.Push({x, y, z});
		Res.Push({x, -y, z});
	}
	return Res;
}

// UDynamicMesh *UViewFinderFunctions::AppendViewFrustum(
// 	UDynamicMesh *TargetMesh,
// 	FGeometryScriptPrimitiveOptions PrimitiveOptions,
// 	FTransform Transform,
// 	float AspectAngle,
// 	float AspectRatio,
// 	float StartDis,
// 	float EndDis,
// 	EGeometryScriptPrimitiveOriginMode Origin,
// 	UGeometryScriptDebug *Debug)
// {
// 	if (TargetMesh == nullptr)
// 	{
// 		UE_LOG(LogTemp, Error, TEXT("UViewFinderFunctions AppendViewFrustum: TargetMesh is Null"));
// 		// UE::Geometry::AppendError(Debug, EGeometryScriptErrorType::InvalidInputs, LOCTEXT("PrimitiveFunctions_AppendBoundingBox", "AppendBoundingBox: TargetMesh is Null"));
// 		return TargetMesh;
// 	}

	
// 	UE::Geometry::FGridBoxMeshGenerator GridBoxGenerator;
// 	GridBoxGenerator.Box = UE::Geometry::FOrientedBox3d( UE::Geometry::FAxisAlignedBox3d(FBox()) );
// 	GridBoxGenerator.EdgeVertices = UE::Geometry::FIndex3i(0, 0, 0);
// 	GridBoxGenerator.bPolygroupPerQuad = (PrimitiveOptions.PolygroupMode == EGeometryScriptPrimitivePolygroupMode::PerQuad);
// 	GridBoxGenerator.Generate();

// 	AppendPrimitive(TargetMesh, &GridBoxGenerator, Transform, PrimitiveOptions, FVector3d::Zero());

// 	return TargetMesh;
// }


bool UViewFinderFunctions::FrustumOverlapComponents(UPrimitiveComponent* Component, const FTransform& ComponentTransform, const TArray<TEnumAsByte<EObjectTypeQuery> > & ObjectTypes, UClass* ComponentClassFilter, const TArray<AActor*>& ActorsToIgnore, TArray<UPrimitiveComponent*>& OutComponents)
{
	UObject dsa;

	OutComponents.Empty();

	if (Component == nullptr) return false;

	FComponentQueryParams Params(SCENE_QUERY_STAT(ComponentOverlapComponents));
	Params.AddIgnoredActors(ActorsToIgnore);

	TArray<FOverlapResult> Overlaps;
	FCollisionObjectQueryParams ObjectParams;
	for (auto Iter = ObjectTypes.CreateConstIterator(); Iter; ++Iter)
	{
		const ECollisionChannel & Channel = UCollisionProfile::Get()->ConvertToCollisionChannel(false, *Iter);
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

	for (int32 OverlapIdx = 0; OverlapIdx <Overlaps.Num(); OverlapIdx++)
	{
		FOverlapResult const& O = Overlaps[OverlapIdx];
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