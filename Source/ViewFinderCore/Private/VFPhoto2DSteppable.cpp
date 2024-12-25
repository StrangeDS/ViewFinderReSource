#include "VFPhoto2DSteppable.h"

void AVFPhoto2DSteppable::BeginPlay()
{
    Super::BeginPlay();

    StepRecorder = GetWorld()->GetSubsystem<UVFStepsRecorderWorldSubsystem>();
    check(StepRecorder);
    StepRecorder->SubmitStep(
        this,
        FVFStepInfo{ToString(EVFPhoto2DState::None)});
}

void AVFPhoto2DSteppable::FoldUp()
{
    Super::FoldUp();

    if (!StepRecorder->bIsRewinding)
    {
        StepRecorder->SubmitStep(
            this,
            FVFStepInfo{ToString(EVFPhoto2DState::Folded)});
    }
}

void AVFPhoto2DSteppable::PlaceDown()
{
    Super::PlaceDown();

    if (!StepRecorder->bIsRewinding)
    {
        StepRecorder->SubmitStep(
            this,
            FVFStepInfo{ToString(EVFPhoto2DState::Placed)});
    }
}

bool AVFPhoto2DSteppable::StepBack_Implementation(FVFStepInfo &StepInfo)
{
    auto CompStep = ToStep(StepInfo.Info);
    switch (CompStep)
    {
    case EVFPhoto2DState::None:
    {
        Destroy();
        break;
    }
    case EVFPhoto2DState::Folded:
    {
        PlaceDown();
        break;
    }
    case EVFPhoto2DState::Placed:
    {
        FoldUp();
        break;
    }
    default:
        return false;
    }

    return true;
}

FString AVFPhoto2DSteppable::ToString(EVFPhoto2DState Step)
{
    return FString::FromInt((int)Step);
}

EVFPhoto2DState AVFPhoto2DSteppable::ToStep(FString String)
{
    return (EVFPhoto2DState)FCString::Atoi(*String);
}
