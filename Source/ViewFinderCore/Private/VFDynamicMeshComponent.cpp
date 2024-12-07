
#include "VFDynamicMeshComponent.h"

#include "VFGeometryFunctions.h"


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

void UVFDynamicMeshComponent::CopyMeshFromComponent(UPrimitiveComponent *Source)
{
    check(Source);
    SourceComponent = Source;

    // 复制网格
    UVFGeometryFunctions::CopyMeshFromComponent(
        Source,
        MeshObject,
        FVF_GeometryScriptCopyMeshFromComponentOptions(),
        true);

    // 复制物理
    bool bUseComplex = SourceComponent->BodyInstance.bSimulatePhysics;
    // 复制碰撞预设, 类型等. 原静态网格体的简单碰撞可能与显示不一致, 自动生成的碰撞并不完美(以及不能智能地选择生成碰撞的设置).
    SetCollisionProfileName(SourceComponent->GetCollisionProfileName());
    if (Source->BodyInstance.bSimulatePhysics)
    {
        // 生成简单碰撞模拟物理, 面数该如何抉择?
        SetComplexAsSimpleCollisionEnabled(false, false);
        UpdateSimlpeCollision();
        SetSimulatePhysics(Source->BodyInstance.bSimulatePhysics);
        SetEnableGravity(Source->IsGravityEnabled());
    }
    else
    {
        // 使用复杂碰撞
        SetComplexAsSimpleCollisionEnabled(true, false);
    }
    SetCollisionEnabled(Source->GetCollisionEnabled());

    SetMaterial(0, Source->GetMaterial(0));
    // TODO: 传递事件. 暂使用Actor接口
}

void UVFDynamicMeshComponent::UpdateSimlpeCollision()
{
    if (bEnableComplexCollision)
        return;
    static FVF_GeometryScriptCollisionFromMeshOptions Options;
    // static FVF_GeometryScriptCollisionFromMeshOptions Options
    // {
    //     false,
    //     EVF_GeometryScriptCollisionGenerationMethod::ConvexHulls,
    //     false,
    //     false,
    //     false,
    //     1.0f,
    //     true,
    //     25,
    //     4,
    //     0.5f,
    //     0.f,
    //     0.1f,
    //     0.1f,
    //     EVF_GeometryScriptSweptHullAxis::Z,
    //     true,
    //     4
    // };
    UVFGeometryFunctions::SetDynamicMeshCollisionFromMesh(
        MeshObject,
        this,
        Options);
}

UPrimitiveComponent *UVFDynamicMeshComponent::GetSourceComponent()
{
    return SourceComponent;
}

bool UVFDynamicMeshComponent::IsSourceVFDMComp()
{
    if (!SourceComponent)
        return false;
    return SourceComponent->GetClass()->IsChildOf(UVFDynamicMeshComponent::StaticClass());
}
