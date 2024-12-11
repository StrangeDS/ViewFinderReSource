#include "VFPhoto2D.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

#include "VFPhoto3D.h"

AVFPhoto2D::AVFPhoto2D()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	// StaticMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	SetActorEnableCollision(false);

    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSelector(
		TEXT("/Game/ViewFinder/StaticMesh/Plane.Plane")
		);
	StaticMeshObject = MeshSelector.Object;
	StaticMesh->SetStaticMesh(StaticMeshObject);
	StaticMesh->SetRelativeRotation(FRotator(0.f, 90.0f, 0.f));
}

void AVFPhoto2D::BeginPlay()
{
	Super::BeginPlay();

	RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget->ResizeTarget(PixelNum, PixelNum);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
}

void AVFPhoto2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AVFPhoto2D::SetPhoto3D(AVFPhoto3D *Photo)
{
	Photo3D = Photo;
}

AVFPhoto3D *AVFPhoto2D::GetPhoto3D()
{
    return Photo3D;
}

void AVFPhoto2D::SetPhoto(USceneCaptureComponent2D *Capturer)
{
	if (!Capturer)
		return;

	if (!MaterialInstance)
	{
		MaterialInstance = StaticMesh->CreateAndSetMaterialInstanceDynamic(0);
		MaterialInstance->SetTextureParameterValue(TextureName, RenderTarget);
	}

	UTextureRenderTarget2D *Original = Capturer->TextureTarget;
	Capturer->TextureTarget = RenderTarget;
	Capturer->CaptureScene();
	Capturer->TextureTarget = Original;
}

void AVFPhoto2D::FoldUp()
{
	if (!Photo3D)
		return;

	Photo3D->FoldUp();
}

void AVFPhoto2D::Preview(const bool &Enabled)
{
	if (!Photo3D)
		return;
	
	Photo3D->SetViewFrustumVisible(true);
}

void AVFPhoto2D::PlaceDown()
{
	if (!Photo3D)
		return;

	Photo3D->SetActorTransform(GetActorTransform());
	Photo3D->PlaceDown();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}
