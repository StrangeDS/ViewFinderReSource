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
	RenderTarget->ResizeTarget(Width, Height);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;

	auto Actor = GetOwner();
	while (Actor) {
		HiddenActors.AddUnique(Actor);
		Actor = Actor->GetParentActor();
	}
}

void UVFPhotoCaptureComponent::Init(UStaticMeshComponent* Mesh, int Index, FName Name)
{
	MaterialInstance = Mesh->CreateAndSetMaterialInstanceDynamic(Index);
	TextureName = Name;
}

void UVFPhotoCaptureComponent::StartDraw()
{
	if (!MaterialInstance)
		return;

	MaterialInstance->SetTextureParameterValue(TextureName, RenderTarget);
	bCaptureEveryFrame = true;
}

void UVFPhotoCaptureComponent::EndDraw()
{
	if (!MaterialInstance)
		return;

	bCaptureEveryFrame = false;
	MaterialInstance->SetTextureParameterValue(TextureName, OriginalTexture);
}
