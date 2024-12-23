#include "VFTransformRecordVolume.h"

#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"

#include "VFStepsRecordInterface.h"

AVFTransformRecordVolume::AVFTransformRecordVolume(const FObjectInitializer &ObjectInitializer)
    : Super(ObjectInitializer)
{
    PrimaryActorTick.bCanEverTick = false;
	Volume = CreateDefaultSubobject<UBoxComponent>("Volume");
    RootComponent = Volume;
}

TArray<UPrimitiveComponent *> AVFTransformRecordVolume::GetComponents()
{
    TArray<UPrimitiveComponent *> OutComponents;
    bool Result = UKismetSystemLibrary::ComponentOverlapComponents(
        Volume,
        Volume->GetComponentTransform(),
        ObjectTypes,
        CompClass,
        {},
        OutComponents);

    for (auto It = OutComponents.CreateIterator(); It; It++)
    {
        if ((*It)->Implements<UVFStepsRecordInterface>())
            It.RemoveCurrent();
    }

    return OutComponents;
}
