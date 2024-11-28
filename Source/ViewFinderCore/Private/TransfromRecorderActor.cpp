#include "TransfromRecorderActor.h"

#include "Kismet/GameplayStatics.h"
#include "Engine/StaticMeshActor.h"

#if WITH_EDITOR
#include "Editor.h"
#include "Subsystems/UnrealEditorSubsystem.h"
#endif

static bool IsTransformEqual(const FTransform &A, const FTransform &B)
{
	return A.Rotator() == B.Rotator() && A.GetLocation() == B.GetLocation() && A.GetScale3D() == B.GetScale3D();
}

ATransfromRecorderActor::ATransfromRecorderActor()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ATransfromRecorderActor::BeginPlay()
{
	Super::BeginPlay();
	
	ReCollectComponents_Implementation();
}

void ATransfromRecorderActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ATransfromRecorderActor::ReCollectComponents_Implementation()
{
	UE_LOG(LogTemp, Warning, TEXT("ReCollectComponents_Implementation."));
	// if (!GEditor) {
	// 	UE_LOG(LogTemp, Warning, TEXT("ReCollectComponents_Implementation noly calls in Editor."));
	// 	return;
	// }
	// UUnrealEditorSubsystem* EditorSubsystem = GEditor->GetEditorSubsystem<UUnrealEditorSubsystem>();
	// if (!EditorSubsystem) {
	// 	UE_LOG(LogTemp, Warning, TEXT("Get null UUnrealEditorSubsystem."));
	// 	return;
	// }
	// UWorld* World = EditorSubsystem->GetEditorWorld();
	// if (!World) {
	// 	UE_LOG(LogTemp, Warning, TEXT("Get null EditorWorld."));
	// 	return;
	// }
	
	// UWorld* World = GEditor->GetPIEWorldContext()->World();
	// GetUnrealEditorSubsystem();

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(this, TSubclassOf<AStaticMeshActor>(), Actors);
	UE_LOG(LogTemp, Warning, TEXT("doing: %i"), Actors.Num());
	Components.Reserve(Actors.Num());
	for (const auto &Actor : Actors) {
		Components.Emplace(Actor->GetRootComponent());
	}
	
	TransMap.Reserve(Components.Num());
	for (const TObjectPtr<USceneComponent> &Comp : Components)
	{
		TransMap.Add(Comp, Comp->GetComponentTransform());
	}
	UE_LOG(LogTemp, Warning, TEXT("finished: %i"), TransMap.Num());
}

FStepInfo ATransfromRecorderActor::MakeStepInfo_Implementation()
{
    return FStepInfo();
	
	for (const TObjectPtr<USceneComponent> &Comp : Components)
	{
		if (!IsTransformEqual(TransMap[Comp], Comp->GetComponentTransform()))
		{
			;
		}
	}
}

bool ATransfromRecorderActor::StepBack_Implementation(FString &Step)
{
    return false;
}
