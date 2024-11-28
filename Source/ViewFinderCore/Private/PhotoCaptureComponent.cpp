#include "PhotoCaptureComponent.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Materials/MaterialInstanceDynamic.h"

UPhotoCaptureComponent::UPhotoCaptureComponent()
{
	// 应当在蓝图中按需求设置这些属性
	// bCaptureEveryFrame = false;
	// bCaptureOnMovement = false;
	// bAlwaysPersistRenderingState = false;
}

void UPhotoCaptureComponent::BeginPlay()
{
	Super::BeginPlay();

	Target2D = NewObject<UTextureRenderTarget2D>(this);
	this->TextureTarget = Target2D;
	Target2D->ResizeTarget(Width, Height);
	Target2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;

	// UTexture2D *Texture = 
    Target2D->ConstructTexture2D(this, TextureName.ToString(), EObjectFlags::RF_NoFlags);
}


void UPhotoCaptureComponent::Init(UStaticMeshComponent *Mesh, int Index, FName Name)
{
	TextureName = Name;
	MaterialInstance = Mesh->CreateAndSetMaterialInstanceDynamic(Index);
	OriginalTexture = MaterialInstance->K2_GetTextureParameterValue(TextureName);
}

void UPhotoCaptureComponent::DrawImage()
{
	if (!bUsingInstance)
	{
		if (MaterialInstance)
		{
			MaterialInstance->SetTextureParameterValue(TextureName, Target2D);
			bUsingInstance = true;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DrawImage has no MaterialInstance"));
		}
	}
	if (bUsingInstance)
		CaptureScene();
}

void UPhotoCaptureComponent::ResetImage()
{
	if (bUsingInstance)
	{

		if (MaterialInstance)
		{
			MaterialInstance->SetTextureParameterValue(TextureName, OriginalTexture);
			bUsingInstance = false;
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("DrawImage has no MaterialInstance"));
		}
	}
}
