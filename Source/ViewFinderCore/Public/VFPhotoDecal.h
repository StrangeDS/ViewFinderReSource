#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VFPhotoDecal.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FAVFPhotoDecalDelegate);

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API AVFPhotoDecal : public AActor
{
	GENERATED_BODY()

public:
	AVFPhotoDecal();

	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void DrawDecal();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void Replace();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void Restore();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetDecalEnabled(bool Enabled);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void SetChildActorsEnabled(bool Enabled);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<class UVFPhotoCaptureComponent> PhotoCapture;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TArray<AActor *> ChildActors;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<class UDecalComponent> Decal;

	UPROPERTY()
	TObjectPtr<UMaterialInterface> Matirial;
	
	UPROPERTY(BlueprintAssignable, Category = "ViewFinder")
	FAVFPhotoDecalDelegate OnReplace;
	
	UPROPERTY(BlueprintAssignable, Category = "ViewFinder")
	FAVFPhotoDecalDelegate OnRestore;
};
