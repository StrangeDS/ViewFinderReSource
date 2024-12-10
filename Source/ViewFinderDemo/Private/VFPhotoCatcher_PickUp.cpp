#include "VFPhotoCatcher_PickUp.h"

#include "EnhancedInputSubsystems.h"
#include "Camera/CameraComponent.h"

bool AVFPhotoCatcher_PickUp::Interact_Implementation(APlayerController *Controller)
{
    EnableInteract(false);
    Execute_EndAiming(this, Controller);
    PlayerController = Controller;
    Pawn = PlayerController->GetPawn();
    // 需要根据角色重写
    if (auto ToAttach = Pawn->GetComponentByClass<UCameraComponent>())
        PickUp(ToAttach);
    return true;
}

void AVFPhotoCatcher_PickUp::TakeAPhoto_Implementation()
{
    if (!bReady)
        return;

    Super::TakeAPhoto_Implementation();
}

void AVFPhotoCatcher_PickUp::CloseToPreview_Implementation()
{
    Super::CloseToPreview_Implementation();

    GetWorldTimerManager().SetTimer(
        PreviewTimeHandle, [this]()
        {
            bReady = true;
            SetViewFrustumVisible(true);
        },
        TimeOfClose,
        false);
}

void AVFPhotoCatcher_PickUp::LeaveFromPreview_Implementation()
{
    Super::LeaveFromPreview_Implementation();

    GetWorldTimerManager().ClearTimer(PreviewTimeHandle);
    bReady = false;
    SetViewFrustumVisible(false);
}

void AVFPhotoCatcher_PickUp::PickUp_Implementation(USceneComponent *ToAttach)
{
    if (bPickedUp)
        return;

    AttachToComponent(ToAttach, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
    RootComponent->SetRelativeLocation(FVector(10.0f, 20.0f, 0.0f)); // 用于测试

    if (HoldingMappingContext)
    {
        auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (Subsystem)
            Subsystem->AddMappingContext(HoldingMappingContext, 1);
        EnableInput(PlayerController);
    }
    bPickedUp = true;
}

void AVFPhotoCatcher_PickUp::DropDown_Implementation()
{
    if (!bPickedUp)
        return;

    if (HoldingMappingContext)
    {
        DisableInput(PlayerController);
        auto Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
        if (Subsystem)
            Subsystem->RemoveMappingContext(HoldingMappingContext);
    }
    DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

    Pawn = nullptr;
    PlayerController = nullptr;
    bPickedUp = false;
    EnableInteract(true);
}
