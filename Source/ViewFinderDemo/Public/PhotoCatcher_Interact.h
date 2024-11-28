#pragma once

#include "CoreMinimal.h"
#include "PhotoCatcher.h"

#include "VF_InteractInterface.h"

#include "PhotoCatcher_Interact.generated.h"

class UUserWidget;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERDEMO_API APhotoCatcher_Interact : public APhotoCatcher, public IVF_InteractInterface
{
	GENERATED_BODY()


protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TSubclassOf<UUserWidget> HintUMGClass;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UUserWidget> HintUMG;

public:
	virtual bool StartAiming_Implementation(APlayerController* Controller);

	virtual bool EndAiming_Implementation(APlayerController* Controller);

	virtual bool Interact_Implementation(APlayerController* Controller);
};
