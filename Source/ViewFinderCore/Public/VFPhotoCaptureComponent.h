#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "VFPhotoCaptureComponent.generated.h"

class UTextureRenderTarget2D;
class UMaterialInstanceDynamic;

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UVFPhotoCaptureComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

	UVFPhotoCaptureComponent();

public:
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void Init(class UStaticMeshComponent *Mesh,
			  float AspectRatioIn = 1.777778f,
			  int Index = 0,
			  FName TextureNameIn = TEXT("Texture"),
			  FName RatioNameIn = TEXT("AspectRatio"));

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void StartDraw();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void EndDraw();

public:
	UPROPERTY(EditAnywhere, Category = "ViewFinder")
	int Width = 1024;

	UPROPERTY(VisibleAnywhere, Category = "ViewFinder")
	float AspectRatio = 16.0f / 9;

	UPROPERTY(VisibleAnywhere, Category = "ViewFinder")
	FName TextureName = TEXT("Texture");

	UPROPERTY(VisibleAnywhere, Category = "ViewFinder")
	FName RatioName = TEXT("AspectRatio");

protected:
	UPROPERTY(VisibleAnywhere, Category = "ViewFinder")
	TObjectPtr<UTextureRenderTarget2D> RenderTarget;

	UPROPERTY(VisibleAnywhere, Category = "ViewFinder")
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;

	UPROPERTY(VisibleAnywhere, Category = "ViewFinder")
	TObjectPtr<UTexture> OriginalTexture;
};
