#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "VFHelperComponent.generated.h"


// 拍照前/后
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorBeginTakenInPhotoSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorEndTakenInPhotoSignature);

// 照片放置前/后
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorBeginPlacedFromPhotoSignature);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FActorEndPlacedFromPhotoSignature);

UCLASS(Blueprintable, ClassGroup = (ViewFinder), meta = (BlueprintSpawnableComponent))
class VIEWFINDERCORE_API UVFHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UVFHelperComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	// 能否被拍入照片
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	bool bCanBeTakenInPhoto = false;
	
	// 能否被放置的照片覆盖(差集)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ViewFinder")
	bool bCanBePlacedByPhoto = true;

public:
	// 动态多播
	void BeginActorTakenInPhoto();
	void EndActorTakenInPhoto();
	void BeginActorPlacedFromPhoto();
	void EndActorPlacedFromPhoto();

	FActorBeginTakenInPhotoSignature ActorBeginTakenInPhoto;
	FActorEndTakenInPhotoSignature EndTakenInPhotoSignature;
	FActorBeginPlacedFromPhotoSignature BeginPlacedFromPhotoSignature;
	FActorEndPlacedFromPhotoSignature EndPlacedFromPhotoSignature;
};
