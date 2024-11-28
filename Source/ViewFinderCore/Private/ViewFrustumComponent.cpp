#include "ViewFrustumComponent.h"

#include "GeometryScript/MeshBasicEditFunctions.h"

UViewFrustumComponent::UViewFrustumComponent()
{
}

// 默认实现: 从简单盒上修改点位置.
void UViewFrustumComponent::GenerateViewFrustum_Implementation(float Angle, float AspectRatio, float StartDis, float EndDis)
{
    check(Angle > 0.0f && Angle < 360.0f);
    check(AspectRatio > 0.0f && AspectRatio < 10.0f);
    check(StartDis > 0.0f);
    check(EndDis > 0.0f && StartDis < EndDis);

    FTransform Transform;
    UGeometryScriptLibrary_MeshPrimitiveFunctions::AppendBox(MeshObject, PrimitiveOptions, Transform);

    TArray<FVector> Positions;
    {
        Positions.Reserve(8);
        auto CalculatePositions = [&](float Distance) {
            float x = Distance;
            float y = Distance * tanf(FMath::DegreesToRadians(Angle) / 2);
            float z = y / AspectRatio;
            Positions.Push({x, -y, -z});
            Positions.Push({x, y, -z});
            Positions.Push({x, y, z});
            Positions.Push({x, -y, z});
        };
        CalculatePositions(StartDis);
        CalculatePositions(EndDis);
    }
    for (int i = 0; i < 8; i++) {
        bool Success;
        UGeometryScriptLibrary_MeshBasicEditFunctions::SetVertexPosition(MeshObject, i, Positions[i], Success, i != 7);
    }
    UGeometryScriptLibrary_CollisionFunctions::SetDynamicMeshCollisionFromMesh(MeshObject, this, CollisionOptions);
    
    if (Matirial) SetMaterial(0, Matirial);
}

void UViewFrustumComponent::RegenerateViewFrustum(float Angle, float AspectRatio, float StartDis, float EndDis)
{
    MeshObject->Reset();

    GenerateViewFrustum(Angle, AspectRatio, StartDis, EndDis);
}
