#include "StepsRecordInterface.h"

FStepInfo IStepsRecordInterface::MakeStepInfo_Implementation()
{
    return FStepInfo();
}

bool IStepsRecordInterface::StepBack_Implementation(FString &Step)
{
    return false;
}

bool IStepsRecordInterface::WhetherToTick_Implementation()
{
    return false;
}
