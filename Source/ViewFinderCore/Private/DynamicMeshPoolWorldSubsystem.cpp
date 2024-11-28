#include "DynamicMeshPoolWorldSubsystem.h"

#include "GeometryScript/GeometryScriptTypes.h"
#include "GeometryScript/SceneUtilityFunctions.h"

UDynamicMeshPoolWorldSubsystem::UDynamicMeshPoolWorldSubsystem()
{
    PlacingPool = CreateDefaultSubobject<UDynamicMeshPool>(TEXT("PlacingPool"));
    ComputingPool = CreateDefaultSubobject<UDynamicMeshPool>(TEXT("ComputingPool"));
}

// 基于UDynamicMeshPool::RequestMesh()，但其中有1000的默认上限，超出上限会将池中所有回收。
UDynamicMesh *UDynamicMeshPoolWorldSubsystem::RequestPlacingMesh(UPrimitiveComponent *PromitiveComponent)
{
    if (!PromitiveComponent) return nullptr;

    UDynamicMesh *Mesh = PlacingPool->RequestMesh();

    FTransform WorldTrans;
    EGeometryScriptOutcomePins CopyResult;
    UGeometryScriptLibrary_SceneUtilityFunctions::CopyMeshFromComponent(
        PromitiveComponent,
        Mesh,
        FGeometryScriptCopyMeshFromComponentOptions(),
        true,
        WorldTrans,
        CopyResult,
        nullptr);

    if (CopyResult == EGeometryScriptOutcomePins::Failure) {
        UE_LOG(LogTemp, Warning, TEXT("UDynamicMeshPoolWorldSubsystem::RequestMesh() fails to copy."));
    }

    return Mesh;
}

// 基于UDynamicMeshPool::RequestMesh()，但其中有1000的默认上限，超出上限会将池中所有回收。
UDynamicMesh *UDynamicMeshPoolWorldSubsystem::RequestComputingMesh(UPrimitiveComponent *PromitiveComponent)
{
    UDynamicMesh *Mesh = ComputingPool->RequestMesh();
    if (!PromitiveComponent)
        return Mesh;

    FTransform WorldTrans;
    EGeometryScriptOutcomePins CopyResult;
    UGeometryScriptLibrary_SceneUtilityFunctions::CopyMeshFromComponent(
        PromitiveComponent,
        Mesh,
        FGeometryScriptCopyMeshFromComponentOptions(),
        true,
        WorldTrans,
        CopyResult,
        nullptr);

    if (CopyResult == EGeometryScriptOutcomePins::Failure) {
        UE_LOG(LogTemp, Warning, TEXT("UDynamicMeshPoolWorldSubsystem::RequestMesh() fails to copy."));
    }

    return Mesh;
}