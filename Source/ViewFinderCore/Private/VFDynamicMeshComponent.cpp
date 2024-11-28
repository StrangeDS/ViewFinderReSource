
#include "VFDynamicMeshComponent.h"

#include "GeometryScript/SceneUtilityFunctions.h"

UVFDynamicMeshComponent::UVFDynamicMeshComponent(const FObjectInitializer& ObjectInitializer)
{
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
