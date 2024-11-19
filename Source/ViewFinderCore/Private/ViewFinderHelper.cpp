// Fill out your copyright notice in the Description page of Project Settings.

#include "ViewFinderHelper.h"

bool FViewFinder::CheckIsTargetInSightCone(const FVector &StartLocation, const FVector &ConeDirectionNormal, float PeripheralVisionAngleCos, float ConeDirectionBackwardOffset, float NearClippingRadiusSq, float const FarClippingRadiusSq, const FVector &TargetLocation)
{
    const FVector BaseLocation = FMath::IsNearlyZero(ConeDirectionBackwardOffset) ? StartLocation : StartLocation - ConeDirectionNormal * ConeDirectionBackwardOffset;
		const FVector ActorToTarget = TargetLocation - BaseLocation;
		const FVector::FReal DistToTargetSq = ActorToTarget.SizeSquared();
		if (DistToTargetSq <= FarClippingRadiusSq && DistToTargetSq >= NearClippingRadiusSq)
		{
			// Will return true if squared distance to Target is smaller than SMALL_NUMBER
			if (DistToTargetSq < SMALL_NUMBER)
			{
				return true;
			}
			
			// Calculate the normal here instead of calling GetUnsafeNormal as we already have the DistToTargetSq (optim)
			const FVector DirectionToTargetNormal = ActorToTarget * FMath::InvSqrt(DistToTargetSq);

			return FVector::DotProduct(DirectionToTargetNormal, ConeDirectionNormal) > PeripheralVisionAngleCos;
		}

		return false;

	// UDynamicMeshComponent dssad;
	// dssad->GetBodyInstance();
	// dssad->GetAllPhysicsObjects();
	// ComponentOverlapMulti();
}

TArray<FVector> FViewFinder::CalculateViewFrustumPostions(float Angle, float AspectRatio, float StartDis, float EndDis)
{
	TArray<FVector> Res;
	Res.Reserve(8);
	float x = 0.f, y = 0.f, z = 0.f;
	{
		x = StartDis;
		y = StartDis * tanf(Angle / 2);
		z = y / AspectRatio;
		Res.Push({x, -y, -z});
		Res.Push({x, y, -z});
		Res.Push({x, y, z});
		Res.Push({x, -y, z});
	}
	{
		x = EndDis;
		y = EndDis * tanf(Angle / 2);
		z = y / AspectRatio;
		Res.Push({x, -y, -z});
		Res.Push({x, y, -z});
		Res.Push({x, y, z});
		Res.Push({x, -y, z});
	}
	return Res;
}
