#pragma once

#include "CoreMinimal.h"
#include "Components/SceneCaptureComponent2D.h"
#include "PhotoCaptureComponent.generated.h"

class UTextureRenderTarget2D;
class UMaterialInstanceDynamic;

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UPhotoCaptureComponent : public USceneCaptureComponent2D
{
	GENERATED_BODY()

	UPhotoCaptureComponent();

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

	bool bUsingInstance = false;

	UPROPERTY()
	UTextureRenderTarget2D* Target2D;
	
	UPROPERTY()
	UMaterialInstanceDynamic* MaterialInstance;
	
	FName TextureName;
	UTexture* OriginalTexture;
};
