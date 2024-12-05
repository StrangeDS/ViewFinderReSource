
#include "VFDynamicMeshComponent.h"


UVFDynamicMeshComponent::UVFDynamicMeshComponent(const FObjectInitializer &ObjectInitializer)
{
}

void UVFDynamicMeshComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    auto PoolSystem = GetWorld()->GetSubsystem<UDynamicMeshPoolWorldSubsystem>();
    check(PoolSystem);
    switch (MeshType)
    {
    case ViewFinder::Placing:
        PoolSystem->ReturnPlacingMesh(MeshObject);
        break;
    case ViewFinder::Computing:
        PoolSystem->ReturnComputingMesh(MeshObject);
        break;
    case ViewFinder::None:
    default:
        break;
    }

    Super::EndPlay(EndPlayReason);
}

void UVFDynamicMeshComponent::SetDynamicMeshFromPool(UDynamicMesh *Mesh, EVF_MeshType Type)
{
    MeshType = Type;
    SetDynamicMesh(Mesh);
}

bool UVFDynamicMeshComponent::HasSourceComponent()
{
    return bool(SourceComponent);
}

UPrimitiveComponent *UVFDynamicMeshComponent::GetSourceComponent()
{
    return SourceComponent;
}

void UVFDynamicMeshComponent::SetSourceComponent(UPrimitiveComponent *PrimitiveComponent)
{
    SourceComponent = PrimitiveComponent;
}
