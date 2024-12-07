#include "DynamicMeshPoolWorldSubsystem.h"

#include "VFGeometryFunctions.h"

UDynamicMeshPoolWorldSubsystem::UDynamicMeshPoolWorldSubsystem()
{
    PlacingPool = CreateDefaultSubobject<UDynamicMeshPool>(TEXT("PlacingPool"));
    ComputingPool = CreateDefaultSubobject<UDynamicMeshPool>(TEXT("ComputingPool"));
}

// 基于UDynamicMeshPool::RequestMesh()，但其中有1000的默认上限，超出上限会将池中所有回收。
UDynamicMesh *UDynamicMeshPoolWorldSubsystem::RequestPlacingMesh(UPrimitiveComponent *PrimitiveComponent)
{
    if (!PrimitiveComponent) return nullptr;

    UDynamicMesh *Mesh = PlacingPool->RequestMesh();

    UVFGeometryFunctions::CopyMeshFromComponent(
        PrimitiveComponent,
        Mesh,
        FVF_GeometryScriptCopyMeshFromComponentOptions(),
        true);

    return Mesh;
}

void UDynamicMeshPoolWorldSubsystem::ReturnPlacingMesh(UDynamicMesh* DynamicMesh)
{
    PlacingPool->ReturnMesh(DynamicMesh);
}

// 基于UDynamicMeshPool::RequestMesh()，但其中有1000的默认上限，超出上限会将池中所有回收。
UDynamicMesh *UDynamicMeshPoolWorldSubsystem::RequestComputingMesh(UPrimitiveComponent *PrimitiveComponent)
{
    UDynamicMesh *Mesh = ComputingPool->RequestMesh();
    if (!PrimitiveComponent)
        return Mesh;

    UVFGeometryFunctions::CopyMeshFromComponent(
        PrimitiveComponent,
        Mesh,
        FVF_GeometryScriptCopyMeshFromComponentOptions(),
        true);

    return Mesh;
}

void UDynamicMeshPoolWorldSubsystem::ReturnComputingMesh(UDynamicMesh* DynamicMesh)
{
    ComputingPool->ReturnMesh(DynamicMesh);
}
