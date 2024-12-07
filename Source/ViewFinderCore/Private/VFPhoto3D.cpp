#include "VFPhoto3D.h"

#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

#include "Components/StaticMeshComponent.h"
#include "VFDynamicMeshComponent.h"

AVFPhoto3D::AVFPhoto3D()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent->SetMobility(EComponentMobility::Movable);
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
}

void AVFPhoto3D::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget->ResizeTarget(PixelNum, PixelNum);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;

	// RenderTarget->RenderTargetFormat = Format;
	// Target2D->ConstructTexture2D(this, TextureName.ToString(), EObjectFlags::RF_NoFlags); // necessary?
	MaterialInstance = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
	// MaterialInstance->SetTextureParameterValue(TextureName, RenderTarget);
}

void AVFPhoto3D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVFPhoto3D::FoldUp(const bool &NeedHide)
{
	if (State == EVF_PhotoState::Folded)
		return;
	State = EVF_PhotoState::Folded;

	if (!NeedHide)
	{
		static FVector Distance(30000.0f, 0.f, 0.f);
		SetActorLocation(Distance);
	}

	TArray<AActor *> Actors;
	GetAttachedActors(Actors, true, true);
	for (const auto &Actor : Actors)
	{
		Actor->SetActorEnableCollision(false);
		if (NeedHide)
			Actor->SetActorHiddenInGame(true);
	}
}

void AVFPhoto3D::PlaceDown(FTransform Transform)
{
	if (State == EVF_PhotoState::Placed)
		return;
	State = EVF_PhotoState::Placed;

	SetActorTransform(Transform); // 关心缩放?
	TArray<AActor *> Actors;
	GetAttachedActors(Actors, true, true);
	for (const auto &Actor : Actors)
	{
		Actor->SetActorEnableCollision(true);
		Actor->SetActorHiddenInGame(false);
	}
}

void AVFPhoto3D::GetVFDMComps(TArray<UVFDynamicMeshComponent *> &Comps)
{
	GetComponents<UVFDynamicMeshComponent>(Comps);
}

void AVFPhoto3D::GetCaptured(USceneCaptureComponent2D *Capturer)
{
	if (!Capturer)
		return;

	UTextureRenderTarget2D *Original = Capturer->TextureTarget;
	Capturer->TextureTarget = RenderTarget;
	Capturer->CaptureScene();
	Capturer->TextureTarget = Original;
}