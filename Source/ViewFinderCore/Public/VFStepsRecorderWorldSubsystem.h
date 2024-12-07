#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "VFStepsRecorderWorldSubsystem.generated.h"

// using namespace ViewFinder;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API UVFStepsRecorderWorldSubsystem : public UTickableWorldSubsystem
{
	GENERATED_BODY()
	
	virtual TStatId GetStatId() const override;
};
