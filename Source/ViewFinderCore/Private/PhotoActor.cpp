#include "PhotoActor.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

#include "Components/StaticMeshComponent.h"
#include "VFDynamicMeshComponent.h"

APhotoActor::APhotoActor()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void APhotoActor::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget->ResizeTarget(PixelNum, PixelNum);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
	;
	// RenderTarget->RenderTargetFormat = Format;
	// Target2D->ConstructTexture2D(this, TextureName.ToString(), EObjectFlags::RF_NoFlags); // necessary?
	MaterialInstance = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
	MaterialInstance->SetTextureParameterValue(TextureName, RenderTarget);
}

void APhotoActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APhotoActor::SetVFDMComps(const TArray<UVFDynamicMeshComponent *> &Comps)
{
	VFDMComps.Reset();
	for (const auto &Comp : Comps)
	{
		VFDMComps.Emplace(TObjectPtr<UVFDynamicMeshComponent>(Comp));
	}
}

void APhotoActor::GetCaptured(USceneCaptureComponent2D *Capturer)
{
	if (!Capturer)
		return;

	UTextureRenderTarget2D *Original = Capturer->TextureTarget;
	Capturer->TextureTarget = RenderTarget;
	Capturer->CaptureScene();
	Capturer->TextureTarget = Original;
}