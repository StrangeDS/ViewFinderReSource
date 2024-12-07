#include "VFStepsRecordInterface.h"

FVFStepInfo IVFStepsRecordInterface::MakeStepInfo_Implementation()
{
    return FVFStepInfo();
}

bool IVFStepsRecordInterface::StepBack_Implementation(FString &Step)
{
    return false;
}

bool IVFStepsRecordInterface::WhetherToTick_Implementation()
{
    return false;
}
