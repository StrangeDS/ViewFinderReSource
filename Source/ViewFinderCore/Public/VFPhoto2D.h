#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VFPhoto2D.generated.h"

class AVFPhoto3D;
class UTextureRenderTarget2D;
class UVFDynamicMeshComponent;

UCLASS(Blueprintable, ClassGroup = (ViewFinder))
class VIEWFINDERCORE_API AVFPhoto2D : public AActor
{
	GENERATED_BODY()

public:
	AVFPhoto2D();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetPhoto3D(AVFPhoto3D *Photo);

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	AVFPhoto3D *GetPhoto3D();

	UFUNCTION(BlueprintCallable, Category = "ViewFinder")
	void SetPhoto(USceneCaptureComponent2D *Capturer);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void FoldUp();

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void Preview(const bool &Enabled);

	UFUNCTION(BlueprintCallable, CallInEditor, Category = "ViewFinder")
	void PlaceDown();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<class UStaticMeshComponent> StaticMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	int PixelNum = 1024;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	FName TextureName = TEXT("Texture");

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UTextureRenderTarget2D> RenderTarget = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<UMaterialInstanceDynamic> MaterialInstance = nullptr;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "ViewFinder")
	TObjectPtr<AVFPhoto3D> Photo3D;
};
