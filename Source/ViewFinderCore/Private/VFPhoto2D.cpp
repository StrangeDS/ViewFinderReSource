#include "VFPhoto2D.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/TextureRenderTarget2D.h"
#include "Components/SceneCaptureComponent2D.h"

#include "VFPhoto3D.h"
#include "VFHelperComponent.h"
#include "VFFunctions.h"

AVFPhoto2D::AVFPhoto2D() : Super()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("IgnoreOnlyPawn"));
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSelector(
		TEXT("/Game/ViewFinder/StaticMesh/Plane.Plane")
		);
	StaticMeshObject = MeshSelector.Object;
	StaticMesh->SetStaticMesh(StaticMeshObject);
	StaticMesh->SetRelativeRotation(FRotator(0.f, 90.0f, 0.f));

	Helper = CreateDefaultSubobject<UVFHelperComponent>("Helper");
	Helper->bCanBePlacedByPhoto = false;
}

void AVFPhoto2D::BeginPlay()
{
	Super::BeginPlay();
	
	Helper->OnCopyAfterCopiedForPhoto.AddUniqueDynamic(this, &AVFPhoto2D::CopyPhoto3D);

	RenderTarget = NewObject<UTextureRenderTarget2D>(this);
	RenderTarget->ResizeTarget(PixelNum, PixelNum);
	RenderTarget->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
}

void AVFPhoto2D::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AVFPhoto2D::SetActorHiddenInGame(bool bNewHidden)
{
	Super::SetActorHiddenInGame(bNewHidden);

	SetActorEnableCollision(!bNewHidden);
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

	if (State == EVFPhoto2DState::Folded)
		return;
	State = EVFPhoto2DState::Folded;

	Photo3D->FoldUp();
}

void AVFPhoto2D::Preview(const FTransform& WorldTrans, const bool &Enabled)
{
	if (!Photo3D)
		return;
	
	Photo3D->SetActorTransform(WorldTrans);
	Photo3D->SetViewFrustumVisible(Enabled);
}

void AVFPhoto2D::PlaceDown()
{
	if (!Photo3D)
		return;

	if (State == EVFPhoto2DState::Placed)
		return;
	State = EVFPhoto2DState::Placed;

	Photo3D->PlaceDown();
	SetActorHiddenInGame(true);
	SetActorEnableCollision(false);
}
