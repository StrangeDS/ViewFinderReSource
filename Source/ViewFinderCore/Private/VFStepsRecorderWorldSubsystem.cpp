#include "VFStepsRecorderWorldSubsystem.h"

#include "Kismet/GameplayStatics.h"

TStatId UVFStepsRecorderWorldSubsystem::GetStatId() const
{
    RETURN_QUICK_DECLARE_CYCLE_STAT(UVFStepsRecorderWorldSubsystem, STATGROUP_Tickables);
}

void UVFStepsRecorderWorldSubsystem::Initialize(FSubsystemCollectionBase &Collection)
{
    Super::Initialize(Collection);
}

void UVFStepsRecorderWorldSubsystem::Deinitialize()
{
    Super::Deinitialize();
}

void UVFStepsRecorderWorldSubsystem::OnWorldBeginPlay(UWorld &InWorld)
{
    Super::OnWorldBeginPlay(InWorld);
}

void UVFStepsRecorderWorldSubsystem::Tick(float DeltaTime)
{
    TimeSinceLastTick += bIsRewinding ? DeltaTime * RewindCurFactor : DeltaTime;
    while (TimeSinceLastTick > TickInterval)
    {
        Time += bIsRewinding ? -TickInterval: TickInterval;
        if (bIsRewinding)
        {
            TickBackward(TickInterval);
        }
        else
        {
            TickForward(TickInterval);
        }
        TimeSinceLastTick -= TickInterval;
        OnTickTime.Broadcast(Time);
    }
}

void UVFStepsRecorderWorldSubsystem::TickForward(float DeltaTime)
{
    Time = FMath::Clamp(Time, TIME_MIN, TIME_MAX);

    for (const auto &Target : TickTargets)
    {
        IVFStepsRecordInterface::Execute_TickForward(Target.GetObject(), Time);
    }
}

void UVFStepsRecorderWorldSubsystem::TickBackward(float DeltaTime)
{
    if (Time <= TIME_MIN)
    {
        Time = TIME_MIN;
        EndRewinding();
        return;
    }

    while (!Infos.IsEmpty())
    {
        auto Info = Infos.Last();
        if (Info.Time < Time)
            break;

        IVFStepsRecordInterface::Execute_StepBack(Info.Sender, Info);
        Infos.Pop(false);
    }

    for (const auto &Target : TickTargets)
    {
        IVFStepsRecordInterface::Execute_TickBackward(Target.GetObject(), Time);
    }
}

void UVFStepsRecorderWorldSubsystem::SubmitStep(UObject *Sender, FVFStepInfo &Info)
{
    Info.Sender = Sender;
    Info.Time = Time;
    Infos.Add(Info);
}

void UVFStepsRecorderWorldSubsystem::RegisterTickable(TScriptInterface<IVFStepsRecordInterface> Target)
{
    check(Target.GetObject()->Implements<UVFStepsRecordInterface>());
    TickTargets.AddUnique(Target);
}

void UVFStepsRecorderWorldSubsystem::UnregisterTickable(TScriptInterface<IVFStepsRecordInterface> Target)
{
    TickTargets.RemoveSwap(Target);
}

void UVFStepsRecorderWorldSubsystem::StartRewinding()
{
    if (bIsRewinding)
        return;

    TimeSinceLastTick = 0.f;
    OnStartRewinding.Broadcast(Time);
    bIsRewinding = true;
}

void UVFStepsRecorderWorldSubsystem::EndRewinding()
{
    if (!bIsRewinding)
        return;

    TimeSinceLastTick = 0.f;
    OnEndRewinding.Broadcast(Time);
    bIsRewinding = false;
}

void UVFStepsRecorderWorldSubsystem::RewindToLastKey()
{
    for (int i = Infos.Num() - 1; i >= 0; i--)
    {
        if (Infos[i].bIsKeyFrame)
        {
            float TimeSpan = (Time - Infos[i].Time - TickInterval);
            TimeSpan = FMath::Max(TimeSpan, TIME_MIN);
            TimeSpan = FMath::Min(TimeSpan, TimeOfRewindToLastKey);
            float Speed = TimeSpan / TimeOfRewindToLastKey;
            RewindCurFactor = Speed;
            StartRewinding();
            GetWorld()->GetTimerManager().SetTimer(
                RewindHandle, [this]() {
                    RewindCurFactor = RewindFactor;
                    EndRewinding();
                },
                TimeOfRewindToLastKey,
                false);
            return;
        }
    }
}

const float UVFStepsRecorderWorldSubsystem::TIME_MAX = 1.0e6;

const float UVFStepsRecorderWorldSubsystem::TIME_MIN = 0.f;