#include "VFPhotoDecal.h"

#include "Components/DecalComponent.h"

#include "VFPhotoCaptureComponent.h"

AVFPhotoDecal::AVFPhotoDecal()
{
    RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

    PhotoCapture = CreateDefaultSubobject<UVFPhotoCaptureComponent>(TEXT("PhotoCapture"));
    PhotoCapture->SetupAttachment(RootComponent);
    PhotoCapture->bCaptureEveryFrame = false;
    PhotoCapture->bCaptureOnMovement = false;
    PhotoCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;

    Decal = CreateDefaultSubobject<UDecalComponent>(TEXT("Decal"));
    Decal->SetupAttachment(RootComponent);
    static ConstructorHelpers::FObjectFinder<UMaterialInterface> MaterialSelector(
        TEXT("/Game/ViewFinder/Material/Decal/MDI_Photolize.MDI_Photolize"));
    Matirial = MaterialSelector.Object;
    Decal->SetDecalMaterial(Matirial);
}

void AVFPhotoDecal::BeginPlay()
{
    Super::BeginPlay();

    // 只记录第一级的子Actor
    ChildActors.Reset();
    GetAttachedActors(ChildActors, false, false);

    TArray<AActor *> ShowActors = ChildActors;
    for (auto &Actor : ChildActors)
    {
        Actor->GetAttachedActors(ShowActors, false, true);
    }
    PhotoCapture->ShowOnlyActors = ShowActors;
    PhotoCapture->Init(Decal->CreateDynamicMaterialInstance());
}

void AVFPhotoDecal::DrawDecal()
{
    SetDecalEnabled(false);
    PhotoCapture->DrawAFrame();
}

void AVFPhotoDecal::Replace()
{
    DrawDecal();
    SetDecalEnabled(true);
}

void AVFPhotoDecal::Restore()
{
    SetDecalEnabled(false);
}

void AVFPhotoDecal::SetDecalEnabled(bool Enabled)
{
    Decal->SetHiddenInGame(!Enabled);
    SetChildActorsEnabled(!Enabled);
}

void AVFPhotoDecal::SetChildActorsEnabled(bool Enabled)
{
    for (auto &Actor : ChildActors)
    {
        Actor->SetActorHiddenInGame(!Enabled);
        Actor->SetActorEnableCollision(Enabled);
    }
}
