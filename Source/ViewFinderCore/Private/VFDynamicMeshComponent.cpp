
#include "VFDynamicMeshComponent.h"

#include "VFGeometryFunctions.h"


UVFDynamicMeshComponent::UVFDynamicMeshComponent(const FObjectInitializer &ObjectInitializer)
{
    SetMobility(EComponentMobility::Movable);
}

void UVFDynamicMeshComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    // auto PoolSystem = GetWorld()->GetSubsystem<UVFDynamicMeshPoolWorldSubsystem>();
    // check(PoolSystem);
    // switch (MeshType)
    // {
    // case ViewFinder::Placing:
    //     if (MeshObject) PoolSystem->ReturnPlacingMesh(MeshObject);
    //     break;
    // case ViewFinder::Computing:
    //     if (MeshObject) PoolSystem->ReturnComputingMesh(MeshObject);
    //     break;
    // case ViewFinder::None:
    // default:
    //     break;
    // }

    Super::EndPlay(EndPlayReason);
}

void UVFDynamicMeshComponent::SetDynamicMeshFromPool(UDynamicMesh *Mesh, EVFMeshType Type)
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
        false);

    // 复制物理
    // 复制碰撞预设, 类型等. 原静态网格体的简单碰撞可能与显示不一致, 自动生成的碰撞并不完美(以及不能智能地选择生成碰撞的设置).
    SetCollisionProfileName(SourceComponent->GetCollisionProfileName());
    if (SourceComponent->BodyInstance.bSimulatePhysics)
    {
        // 生成简单碰撞模拟物理, 面数该如何抉择?
        SetComplexAsSimpleCollisionEnabled(false, true);
        UpdateSimlpeCollision();
        // 从静态网格体上复制需要应用物理状态, 但从VFDMComp上复制不需要立即应用
        bSimulatePhysicsRecorder = SourceComponent->BodyInstance.bSimulatePhysics;
        bEnableGravityRecorder = SourceComponent->IsGravityEnabled();
        SetSimulatePhysics(bSimulatePhysicsRecorder);
        SetEnableGravity(bEnableGravityRecorder);
    }
    else
    {
        // 使用复杂碰撞
        SetComplexAsSimpleCollisionEnabled(true, true);
    }
    SetCollisionEnabled(SourceComponent->GetCollisionEnabled());

    // 复制材质
    SetMaterial(0, SourceComponent->GetMaterial(0));
    
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

void UVFDynamicMeshComponent::SetEnabled(bool Enabled)
{
    if (bEnabled == Enabled)
        return;
    bEnabled = Enabled;
    
    if (bEnabled)
    {
        SetSimulatePhysics(bSimulatePhysicsRecorder);
        SetEnableGravity(bEnableGravityRecorder);
    }
    else
    {
        SetSimulatePhysics(false);
        SetEnableGravity(false);
        if (bSimulatePhysicsRecorder)
            AttachToComponent(GetOwner()->GetRootComponent(), FAttachmentTransformRules::KeepWorldTransform);
    }
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
