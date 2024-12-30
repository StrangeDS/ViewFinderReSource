#include "VFPhotoCatcherOnBeginPlay.h"

#include "VFPhoto2D.h"
#include "VFPhotoCaptureComponent.h"

AVFPhotoCatcherOnBeginPlay::AVFPhotoCatcherOnBeginPlay() : Super()
{
    PhotoCapture->bCaptureEveryFrame = false;
    PhotoCapture->bCaptureOnMovement = false;
}

void AVFPhotoCatcherOnBeginPlay::BeginPlay()
{
    Super::BeginPlay();

    if (bOnlyCatchChildActors)
    {
        TArray<AActor *> Actors;
        GetAttachedActors(Actors, true, true);
        PhotoCapture->ShowOnlyActors = Actors;
        PhotoCapture->PrimitiveRenderMode = ESceneCapturePrimitiveRenderMode::PRM_UseShowOnlyList;
    }

    auto Photo2D = TakeAPhoto();
    Photo2D->AddActorLocalTransform(PhotoSpawnPoint);
    Destroy();
}
