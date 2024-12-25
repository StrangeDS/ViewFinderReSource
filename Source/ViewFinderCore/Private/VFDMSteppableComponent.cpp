#include "VFDMSteppableComponent.h"

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
        FVFStepInfo{ToString(UVFDMSteppableCompStep::BeginPlay), true});
}

void UVFDMSteppableComponent::CopyMeshFromComponent(UPrimitiveComponent *Source)
{
    Super::CopyMeshFromComponent(Source);

    StepRecorder->SubmitStep(
        this,
        FVFStepInfo{ToString(UVFDMSteppableCompStep::CopyMeshFromComponent)});

    if (bSimulatePhysicsRecorder)
    {
        StepRecorder->RecordTransform(this);
        StepRecorder->SubmitStep(
            this,
            FVFStepInfo{ToString(UVFDMSteppableCompStep::RegisterToTransformRecorder)});
    }
}

bool UVFDMSteppableComponent::StepBack_Implementation(FVFStepInfo &StepInfo)
{
    auto CompStep = ToStep(StepInfo.Info);
    switch (CompStep)
    {
    case UVFDMSteppableCompStep::BeginPlay:
    {
        if (GetSourceVFDMComp())
        {
            GetOwner()->Destroy();
        }
        else
        {
            SourceComponent->SetHiddenInGame(false);
            SourceComponent->SetCollisionProfileName(GetCollisionProfileName());
            SourceComponent->SetCollisionEnabled(GetCollisionEnabled());
            SourceComponent->SetSimulatePhysics(bSimulatePhysicsRecorder);
            DestroyComponent();
        }
        break;
    }
    case UVFDMSteppableCompStep::CopyMeshFromComponent:
    {
        auto SourceVFDMComp = GetSourceVFDMComp();
        // if (SourceVFDMComp && bNeedUnionToSource)
        if (SourceVFDMComp)
        {
            SetComponentToWorld(SourceVFDMComp->GetComponentToWorld());
            SourceVFDMComp->UnionMeshWithDMComp(this);
        }
        break;
    }
    case UVFDMSteppableCompStep::RegisterToTransformRecorder:
    {
        StepRecorder->UnrecordTransform(this);
        break;
    }
    default:
    return false;
    }

    return true;
}

FString UVFDMSteppableComponent::ToString(UVFDMSteppableCompStep Step)
{
    return FString::FromInt((int)Step);
}

UVFDMSteppableCompStep UVFDMSteppableComponent::ToStep(FString String)
{
    return (UVFDMSteppableCompStep)FCString::Atoi(*String);
}
