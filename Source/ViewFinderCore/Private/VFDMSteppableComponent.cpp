#include "VFDMSteppableComponent.h"

#include "VFDynamicMeshPoolWorldSubsystem.h"

UVFDMSteppableComponent::UVFDMSteppableComponent(const FObjectInitializer &ObjectInitializer)
    : UVFDynamicMeshComponent(ObjectInitializer)
{
}

void UVFDMSteppableComponent::BeginPlay()
{
    Super::BeginPlay();

    StepRecorder = GetWorld()->GetSubsystem<UVFStepsRecorderWorldSubsystem>();
    check(StepRecorder);
    StepRecorder->SubmitStep(
        this,
        FVFStepInfo{
            EnumToString<UVFDMCompStepOperation>(
                UVFDMCompStepOperation::BeginPlay)});
    StepRecorder->RegisterTickable(this);
}

void UVFDMSteppableComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
    if (!Steps.IsEmpty())
    {
        UE_LOG(LogTemp, Warning,
               TEXT("%s UVFDMSteppableComponent::EndPlay has UDynamicMesh not return %i"),
               *GetOwner()->GetName(),
               Steps.Num());
    }

    while (!Steps.IsEmpty())
    {
        MeshPool->ReturnComputingMesh(Steps.Last().Mesh);
        Steps.Pop(false);
    }

    Super::EndPlay(EndPlayReason);
}

UDynamicMesh *UVFDMSteppableComponent::RequestACopiedMesh()
{
    if (ensure(MeshPool))
    {
        auto CopyiedMesh = MeshPool->RequestComputingMesh();
        CopyiedMesh->SetMesh(MeshObject->GetMeshRef());
        return CopyiedMesh;
    }
    return nullptr;
}

void UVFDMSteppableComponent::CopyMeshFromComponent(UPrimitiveComponent *Source)
{
    Super::CopyMeshFromComponent(Source);

    StepRecorder->SubmitStep(
        this,
        FVFStepInfo{EnumToString<UVFDMCompStepOperation>(
            UVFDMCompStepOperation::CopyMeshFromComponent)});

    if (bSimulatePhysicsRecorder)
    {
        StepRecorder->RecordTransform(this);
        StepRecorder->SubmitStep(
            this,
            FVFStepInfo{EnumToString<UVFDMCompStepOperation>(
                UVFDMCompStepOperation::RegisterToTransformRecorder)});
    }
}

void UVFDMSteppableComponent::ReplaceMeshForComponent(UPrimitiveComponent *Source)
{
    Super::ReplaceMeshForComponent(Source);

    StepRecorder->SubmitStep(
        this,
        FVFStepInfo{EnumToString<UVFDMCompStepOperation>(
            UVFDMCompStepOperation::ReplaceMeshForComponent)});
}

void UVFDMSteppableComponent::IntersectMeshWithDMComp(UDynamicMeshComponent *Tool)
{
    Steps.Add(FVFDMCompStep{
        UVFDMCompStepOperation::IntersectMeshWithDMComp,
        RequestACopiedMesh(),
        StepRecorder->Time});

    Super::IntersectMeshWithDMComp(Tool);
}

void UVFDMSteppableComponent::SubtractMeshWithDMComp(UDynamicMeshComponent *Tool)
{
    Steps.Add(FVFDMCompStep{
        UVFDMCompStepOperation::SubtractMeshWithDMComp,
        RequestACopiedMesh(),
        StepRecorder->Time});

    Super::SubtractMeshWithDMComp(Tool);
}

void UVFDMSteppableComponent::UnionMeshWithDMComp(UDynamicMeshComponent *Tool)
{
    Steps.Add(FVFDMCompStep{
        UVFDMCompStepOperation::UnionMeshWithDMComp,
        RequestACopiedMesh(),
        StepRecorder->Time});

    Super::UnionMeshWithDMComp(Tool);
}

void UVFDMSteppableComponent::TickBackward_Implementation(float Time)
{
    while (!Steps.IsEmpty())
    {
        auto &StepInfo = Steps.Last();
        if (StepInfo.Time < Time)
            break;

        switch (StepInfo.Operation)
        {
        case UVFDMCompStepOperation::IntersectMeshWithDMComp:
        case UVFDMCompStepOperation::SubtractMeshWithDMComp:
        case UVFDMCompStepOperation::UnionMeshWithDMComp:
        default:
        {
            MeshObject->SetMesh(StepInfo.Mesh->GetMeshRef());
            MeshPool->ReturnComputingMesh(StepInfo.Mesh);
            break;
        }
        }
        Steps.Pop(false);
    }
}

bool UVFDMSteppableComponent::StepBack_Implementation(FVFStepInfo &StepInfo)
{
    auto CompStep = StringToEnum<UVFDMCompStepOperation>(StepInfo.Info);
    switch (CompStep)
    {
    case UVFDMCompStepOperation::BeginPlay:
    {
        if (GetSourceVFDMComp()) // 复制体上的
            GetOwner()->Destroy();
        else
            DestroyComponent(); // 原网格上的(ReplaceMeshForComponent)创建的
        break;
    }
    case UVFDMCompStepOperation::CopyMeshFromComponent:
    {
        // Source不保存的做法
        // auto SourceVFDMComp = GetSourceVFDMComp();
        // if (SourceVFDMComp)
        // {
        //     SetComponentToWorld(SourceVFDMComp->GetComponentToWorld());
        //     SourceVFDMComp->UnionMeshWithDMComp(this);
        // }
        break;
    }
    case UVFDMCompStepOperation::RegisterToTransformRecorder:
    {
        StepRecorder->UnrecordTransform(this);
        break;
    }
    case UVFDMCompStepOperation::ReplaceMeshForComponent:
    {
        SetEnabled(false);

        SourceComponent->SetHiddenInGame(false);
        SourceComponent->SetCollisionProfileName(GetCollisionProfileName());
        SourceComponent->SetCollisionEnabled(GetCollisionEnabled());
        SourceComponent->SetSimulatePhysics(bSimulatePhysicsRecorder);
        SourceComponent->BodyInstance.bSimulatePhysics = bEnableGravityRecorder;
        break;
    }
    default:
        return false;
    }

    return true;
}