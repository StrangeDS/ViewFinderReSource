#include "ViewFrustumComponent.h"


UViewFrustumComponent::UViewFrustumComponent()
{
    SetComplexAsSimpleCollisionEnabled(false);
}

void UViewFrustumComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHiddenInGame(true);
}

void UViewFrustumComponent::GenerateViewFrustum_Implementation(float Angle, float AspectRatio, float StartDis, float EndDis)
{
    check(Angle > 0.0f && Angle < 360.0f);
    check(AspectRatio > 0.0f && AspectRatio < 10.0f);
    check(StartDis > 0.0f);
    check(EndDis > 0.0f && StartDis < EndDis);

    UVFGeometryFunctions::AppendFrustum(MeshObject, PrimitiveOptions, Angle, AspectRatio, StartDis, EndDis);

    UVFGeometryFunctions::SetDynamicMeshCollisionFromMesh(MeshObject, this, CollisionOptions);
    
    if (Matirial) SetMaterial(0, Matirial);
}

void UViewFrustumComponent::RegenerateViewFrustum(float Angle, float AspectRatio, float StartDis, float EndDis)
{
    MeshObject->Reset();

    GenerateViewFrustum(Angle, AspectRatio, StartDis, EndDis);
}
