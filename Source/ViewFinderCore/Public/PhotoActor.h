// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "StepsRecordInterface.h"

#include "PhotoActor.generated.h"

class UTextureRenderTarget2D;
class UVFDynamicMeshComponent;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API APhotoActor : public AActor, public IStepsRecordInterface
{
	GENERATED_BODY()

public:
	APhotoActor();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

	void SetVFDMComps(const TArray<UVFDynamicMeshComponent *> &Comps);

	void GetCaptured(USceneCaptureComponent2D *Capturer);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	int PixelNum = 1024;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	FName TextureName = TEXT("Texture");

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	UStaticMeshComponent *StaticMesh = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	UTextureRenderTarget2D *RenderTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	UMaterialInstanceDynamic *MaterialInstance = nullptr;

	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	// TEnumAsByte<ETextureRenderTargetFormat> Format;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	TArray<TObjectPtr<UVFDynamicMeshComponent>> VFDMComps;
};
