#include "VFPhoto3DSteppable.h"

void AVFPhoto3DSteppable::BeginPlay()
{
	StepRecorder = GetWorld()->GetSubsystem<UVFStepsRecorderWorldSubsystem>();
    check(StepRecorder);
    StepRecorder->SubmitStep(
        this,
        FVFStepInfo{ToString(EVFPhoto3DState::None)});

	Super::BeginPlay();
}

void AVFPhoto3DSteppable::FoldUp()
{
    Super::FoldUp();

    if (!StepRecorder->bIsRewinding)
    {
        StepRecorder->SubmitStep(
            this,
            FVFStepInfo{ToString(EVFPhoto3DState::Folded)});
    }
}

void AVFPhoto3DSteppable::PlaceDown()
{
    Super::PlaceDown();

    if (!StepRecorder->bIsRewinding)
    {
        StepRecorder->SubmitStep(
            this,
            FVFStepInfo{ToString(EVFPhoto3DState::Placed)});
    }
}

bool AVFPhoto3DSteppable::StepBack_Implementation(FVFStepInfo &StepInfo)
{
    auto CompStep = ToStep(StepInfo.Info);
    switch (CompStep)
    {
    case EVFPhoto3DState::None:
    {
		Destroy();
        break;
    }
    case EVFPhoto3DState::Folded:
    {
		PlaceDown();
        break;
    }
    case EVFPhoto3DState::Placed:
    {
		FoldUp();
        break;
    }
    default:
        return false;
    }

    return true;
}

FString AVFPhoto3DSteppable::ToString(EVFPhoto3DState Step)
{
    return FString::FromInt((int)Step);
}

EVFPhoto3DState AVFPhoto3DSteppable::ToStep(FString String)
{
    return (EVFPhoto3DState)FCString::Atoi(*String);
}
