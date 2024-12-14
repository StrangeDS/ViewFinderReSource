// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VFStepsRecordInterface.h"

#include "VFPhoto3D.generated.h"

UENUM(BlueprintType)
enum class EVFPhotoState : uint8
{
	None,
	Folded,
	Placed
};

class UTextureRenderTarget2D;
class UVFDynamicMeshComponent;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API AVFPhoto3D : public AActor, public IVFStepsRecordInterface
{
	GENERATED_BODY()

public:
	AVFPhoto3D();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void FoldUp();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void PlaceDown();
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void SetViewFrustumVisible(const bool &Visiblity);
	
	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void SetVFDMCompsEnabled(const bool &Enabled);

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	EVFPhotoState State = EVFPhotoState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UStaticMeshComponent> StaticMesh;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UVFViewFrustumComponent> ViewFrustumRecorder;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	// TEnumAsByte<ETextureRenderTargetFormat> Format;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	// TArray<TObjectPtr<UVFDynamicMeshComponent>> VFDMComps;

public:	// 记录属性
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void RecordProperty(
		UVFViewFrustumComponent *ViewFrustum,
		bool OnlyWithHelps,
		const TArray<TEnumAsByte<EObjectTypeQuery>> &ObjectTypes);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	bool bOnlyOverlapWithHelps = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypesToOverlap;
};
