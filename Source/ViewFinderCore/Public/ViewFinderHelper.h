// Written By StrangeDS.

#pragma once

#include "CoreMinimal.h"

namespace FViewFinder
{
	// FAISystem::CheckIsTargetInSightCone();
	bool CheckIsTargetInSightCone(const FVector& StartLocation, const FVector& ConeDirectionNormal, float PeripheralVisionAngleCos,
											   float ConeDirectionBackwardOffset, float NearClippingRadiusSq, float const FarClippingRadiusSq, const FVector& TargetLocation);

	TArray<FVector> CalculateViewFrustumPostions(float Angle, float AspectRatio, float StartDis, float EndDis);
}