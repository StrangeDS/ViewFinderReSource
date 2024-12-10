#include "VFViewFrustumComponent.h"

UVFViewFrustumComponent::UVFViewFrustumComponent()
{
    SetComplexAsSimpleCollisionEnabled(false);
	SetCollisionProfileName(TEXT("ViewFrustum"));
    SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void UVFViewFrustumComponent::BeginPlay()
{
    Super::BeginPlay();

    SetHiddenInGame(true);
}

void UVFViewFrustumComponent::GenerateViewFrustum_Implementation(float Angle, float AspectRatio, float StartDis, float EndDis)
{
    check(Angle > 0.0f && Angle < 360.0f);
    check(AspectRatio > 0.0f && AspectRatio < 10.0f);
    check(StartDis > 0.0f);
    check(EndDis > 0.0f && StartDis < EndDis);

    UVFGeometryFunctions::AppendFrustum(MeshObject, PrimitiveOptions, Angle, AspectRatio, StartDis, EndDis);
    UVFGeometryFunctions::SetDynamicMeshCollisionFromMesh(MeshObject, this, CollisionOptions);
    
}

void UVFViewFrustumComponent::RegenerateViewFrustum(float Angle, float AspectRatio, float StartDis, float EndDis)
{
    MeshObject->Reset();
    GenerateViewFrustum(Angle, AspectRatio, StartDis, EndDis);
}

void UVFViewFrustumComponent::CopyViewFrustum(UVFViewFrustumComponent *Other)
{
    UVFGeometryFunctions::CopyMeshFromComponent(
        Other,
        MeshObject,
        FVF_GeometryScriptCopyMeshFromComponentOptions(),
        false);

	SetCollisionProfileName(TEXT("ViewFrustum"));
    UVFGeometryFunctions::SetDynamicMeshCollisionFromMesh(MeshObject, this, CollisionOptions);
    SetComplexAsSimpleCollisionEnabled(false);
    SetCollisionEnabled(ECollisionEnabled::NoCollision);
    // SetHiddenInGame(false);
}
