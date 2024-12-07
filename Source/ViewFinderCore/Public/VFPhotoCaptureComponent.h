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

	void Init(UStaticMeshComponent *Mesh, int Index, FName Name);

	virtual void DrawImage();

	virtual void ResetImage();

protected:
	UPROPERTY(EditAnywhere, Category = "ViewFinder")
	int Width = 1024;

	UPROPERTY(EditAnywhere, Category = "ViewFinder")
	int Height = 1024;

	UPROPERTY(EditAnywhere, Category = "ViewFinder")
	FName TextureName = TEXT("Temp");

	UPROPERTY()
	TObjectPtr<UTextureRenderTarget2D> Target2D;
	
	UPROPERTY()
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance;
	
	UPROPERTY()
	TObjectPtr<UTexture> OriginalTexture;

	bool bUsingInstance = false;
};
