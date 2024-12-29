#include "VFPhotoCaptureComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

UVFPhotoCaptureComponent::UVFPhotoCaptureComponent()
{
	bCaptureEveryFrame = false;
	bCaptureOnMovement = false;
	bAlwaysPersistRenderingState = false;
}

void UVFPhotoCaptureComponent::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	this->TextureTarget = RenderTarget;
	RenderTarget->InitCustomFormat(Width, Width, PF_FloatRGBA, false);
	RenderTarget->ClearColor = FLinearColor::Black;

	auto Actor = GetOwner();
	while (Actor)
	{
		HiddenActors.AddUnique(Actor);
		Actor = Actor->GetParentActor();
	}
}

void UVFPhotoCaptureComponent::Init(UStaticMeshComponent *Mesh,
									float AspectRatioIn,
									int Index,
									FName TextureNameIn,
									FName RatioNameIn)
{
	MaterialInstance = Mesh->CreateAndSetMaterialInstanceDynamic(Index);
	AspectRatio = AspectRatioIn;
	TextureName = TextureNameIn;
	RatioName = RatioNameIn;
}

void UVFPhotoCaptureComponent::StartDraw()
{
	if (!MaterialInstance)
		return;

	MaterialInstance->SetTextureParameterValue(TextureName, RenderTarget);
	MaterialInstance->SetScalarParameterValue(RatioName, AspectRatio);
	bCaptureEveryFrame = true;
}

void UVFPhotoCaptureComponent::EndDraw()
{
	if (!MaterialInstance)
		return;

	bCaptureEveryFrame = false;
	MaterialInstance->SetTextureParameterValue(TextureName, OriginalTexture);
}
