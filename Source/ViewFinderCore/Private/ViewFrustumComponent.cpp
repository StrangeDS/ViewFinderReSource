#include "ViewFrustumComponent.h"

#include "GeometryScript/CollisionFunctions.h"
#include "GeometryScript/MeshPrimitiveFunctions.h"

UViewFrustumComponent::UViewFrustumComponent()
{
}

void UViewFrustumComponent::GenerateViewFrustum_Implementation()
{
	// C++写不能很好地定义参数，但处于性能考虑预留。
    // AppendBox( 
    // UDynamicMesh* TargetMesh, 
    // FGeometryScriptPrimitiveOptions PrimitiveOptions,
    // FTransform Transform,
    // float DimensionX = 100,
    // float DimensionY = 100,
    // float DimensionZ = 100,
    // int32 StepsX = 0,
    // int32 StepsY = 0,
    // int32 StepsZ = 0,
    // EGeometryScriptPrimitiveOriginMode Origin = EGeometryScriptPrimitiveOriginMode::Base,
    // UGeometryScriptDebug* Debug = nullptr);

    // SetDynamicMeshCollisionFromMesh(
    // UDynamicMesh* FromDynamicMesh, 
    // UDynamicMeshComponent* ToDynamicMeshComponent,
    // FGeometryScriptCollisionFromMeshOptions Options,
    // UGeometryScriptDebug* Debug = nullptr);
}

void UViewFrustumComponent::RegenerateViewFrustum()
{
    MeshObject->Reset();

    GenerateViewFrustum();
}
