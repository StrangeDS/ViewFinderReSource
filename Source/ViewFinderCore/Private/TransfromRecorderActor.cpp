#include "TransfromRecorderActor.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

static bool IsTransformEqual(const FTransform &A, const FTransform &B)
{
	return A.Rotator() == B.Rotator() && A.GetLocation() == B.GetLocation() && A.GetScale3D() == B.GetScale3D();
}

ATransfromRecorderActor::ATransfromRecorderActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ATransfromRecorderActor::BeginPlay()
{
	Super::BeginPlay();
	
	TransMap.Reserve(Components.Num());
	for (const TObjectPtr<USceneComponent> &Comp : Components)
	{
		TransMap[Comp] = Comp->GetComponentTransform();
	}

}

// Called every frame
void ATransfromRecorderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	for (const TObjectPtr<USceneComponent> &Comp : Components)
	{
		if (!IsTransformEqual(TransMap[Comp], Comp->GetComponentTransform()))
		{
			;
		}
	}
}

void ATransfromRecorderActor::ReCollectComponents_Implementation()
{
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<AStaticMeshActor>(), Actors);
	Components.Reserve(Actors.Num());
	for (const auto &Actor : Actors) {
		Components.Emplace(Actor->GetRootComponent());
	}
}

FStepInfo ATransfromRecorderActor::MakeStepInfo_Implementation()
{
    return FStepInfo();
}

bool ATransfromRecorderActor::StepBack_Implementation(FString &Step)
{
    return false;
}
