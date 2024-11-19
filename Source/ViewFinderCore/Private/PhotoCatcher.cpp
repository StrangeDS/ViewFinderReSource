#include "PhotoCatcher.h"

// UPhotoCatcher::UPhotoCatcher()
// {
// }

// void UPhotoCatcher::BeginPlay()
// {
//     Target2D = NewObject<UTextureRenderTarget2D>(this);
//     TextureTarget = Target2D;
// 	Target2D->ResizeTarget(Width, Height);
// 	Target2D->RenderTargetFormat = ETextureRenderTargetFormat::RTF_RGBA8;
// 	Target2D->ConstructTexture2D(this, FName(), EObjectFlags::RF_NoFlags);
// }

// void UPhotoCatcher::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
// {
// 	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

//     if (IsVideoMode) {
//         CaptureScene();
//     }

//     UCameraComponent dsad;
// }

// void UPhotoCatcher::DrawImage()
// {
//     CaptureScene();
//     CatchObjs();
// }

// void UPhotoCatcher::CatchObjs()
// {

// }

// void UPhotoCatcher::UpdateCone()
// {
// 	if (Cone != nullptr)
// 	{
// 		bool bAnythingChanged = false;
// 		const float FrustumDrawDistance = 1000.0f;
// 		if (ProjectionMode == ECameraProjectionMode::Perspective)
// 		{
// 			if (DrawFrustum->FrustumAngle != FieldOfView ||
// 				DrawFrustum->FrustumStartDist != 10.f ||
// 				DrawFrustum->FrustumEndDist != DrawFrustum->FrustumStartDist + FrustumDrawDistance)
// 			{
// 				DrawFrustum->FrustumAngle = FieldOfView;
// 				DrawFrustum->FrustumStartDist = 10.f;
// 				DrawFrustum->FrustumEndDist = DrawFrustum->FrustumStartDist + FrustumDrawDistance;
// 				bAnythingChanged = true;
// 			}
// 		}
// 		else
// 		{
// 			if (DrawFrustum->FrustumAngle != -OrthoWidth ||
// 				DrawFrustum->FrustumStartDist != OrthoNearClipPlane ||
// 				DrawFrustum->FrustumEndDist != FMath::Min(OrthoFarClipPlane - OrthoNearClipPlane, FrustumDrawDistance))
// 			{
// 				DrawFrustum->FrustumAngle = -OrthoWidth;
// 				DrawFrustum->FrustumStartDist = OrthoNearClipPlane;
// 				DrawFrustum->FrustumEndDist = FMath::Min(OrthoFarClipPlane - OrthoNearClipPlane, FrustumDrawDistance);
// 				bAnythingChanged = true;
// 			}
// 		}

// 		if (DrawFrustum->FrustumAspectRatio != AspectRatio)
// 		{
// 			DrawFrustum->FrustumAspectRatio = AspectRatio;
// 			bAnythingChanged = true;
// 		}	
		
// 		if (bAnythingChanged)
// 		{
// 			DrawFrustum->MarkRenderStateDirty();
// 		}
// 	}
// }

// void UPhotoCatcher::OnRegister()
// {
// 	AActor* MyOwner = GetOwner();
// 	if ((MyOwner != nullptr) && !IsRunningCommandlet())
//     {
// 		if (DrawFrustum == nullptr)
// 		{
// 			DrawFrustum = NewObject<UDrawFrustumComponent>(MyOwner, NAME_None, RF_Transactional | RF_TextExportTransient);
// 			DrawFrustum->SetupAttachment(this);
// 			DrawFrustum->SetIsVisualizationComponent(true);
// 			DrawFrustum->CreationMethod = CreationMethod;
// 			DrawFrustum->bFrustumEnabled = bDrawFrustumAllowed;
// 			DrawFrustum->RegisterComponentWithWorld(GetWorld());
// 		}
//     }

// 	RefreshVisualRepresentation();
    
// 	Super::OnRegister();
// }

// void UPhotoCatcher::AddReferencedObjects(UObject *InThis, FReferenceCollector &Collector)
// {
// 	UPhotoCatcher* This = CastChecked<UPhotoCatcher>(InThis);
// 	Collector.AddReferencedObject(This->DrawFrustum);

// 	Super::AddReferencedObjects(InThis, Collector);
// }

// void UPhotoCatcher::OnComponentDestroyed(bool bDestroyingHierarchy)
// {
// 	Super::OnComponentDestroyed(bDestroyingHierarchy);

//     if (DrawFrustum) DrawFrustum->DestroyComponent();
// }
