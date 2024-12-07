// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "VFStepsRecordInterface.h"

#include "VFPhoto3D.generated.h"

UENUM(BlueprintType)
enum class EVF_PhotoState : uint8
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
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void FoldUp(const bool &NeedHide = true);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void PlaceDown(FTransform Transform);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void GetVFDMComps(TArray<UVFDynamicMeshComponent *> &Comps);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void GetCaptured(USceneCaptureComponent2D *Capturer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	int PixelNum = 1024;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	FName TextureName = TEXT("Texture");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	EVF_PhotoState State = EVF_PhotoState::None;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UTextureRenderTarget2D> RenderTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance = nullptr;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	// TEnumAsByte<ETextureRenderTargetFormat> Format;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<TObjectPtr<UVFDynamicMeshComponent>> VFDMComps;
};
