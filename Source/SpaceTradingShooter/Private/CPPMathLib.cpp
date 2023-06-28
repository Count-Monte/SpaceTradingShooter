// Fill out your copyright notice in the Description page of Project Settings.

#include "CPPMathLib.h"
#include "SpaceTradingShooter.h"
#include "Engine.h"
#include <random>

float UCPPMathLib::cmPerUU = 0.01f;
float UCPPMathLib::meterPerUU = UCPPMathLib::cmPerUU * 100.0f;
float UCPPMathLib::kmPerUU = UCPPMathLib::meterPerUU * 1000.0f;

void UCPPMathLib::FindScreenEdgeLocationForWorldLocation(UObject* WorldContextObject, const FVector& InLocation, const float EdgePercent, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen, bool &FacingBackward)
{
	bIsOnScreen = false;
	OutRotationAngleDegrees = 0.f;
	FacingBackward = false;
	FVector ContrivedParallelLocation = InLocation;
	if (!GEngine) return;
	const FVector2D ViewportSize = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY());
	const FVector2D  ViewportCenter = FVector2D(ViewportSize.X / 2, ViewportSize.Y / 2);
	UWorld* World = GEngine->GetWorldFromContextObjectChecked(WorldContextObject);
	if (!World) return;
	APlayerController* PlayerController = (WorldContextObject ? UGameplayStatics::GetPlayerController(WorldContextObject, 0) : NULL);
	if (!PlayerController) return;
	APlayerCameraManager* ViewPointActor = (WorldContextObject ? UGameplayStatics::GetPlayerCameraManager(WorldContextObject, 0) : NULL);
	if (!ViewPointActor) return;
	FVector Location = ViewPointActor->GetCameraLocation();
	FVector Forward = ViewPointActor->GetActorForwardVector();
	FVector Offset = (InLocation - Location).GetSafeNormal();
	float DotProduct = FVector::DotProduct(Forward, Offset);
	bool bLocationIsBehindCamera = (DotProduct < 0);
	if (bLocationIsBehindCamera)
	{
		// For behind the camera situation, I project the objects location to the plane perpendicular to the viewport of the camera
		// then put it a few centimeters forward of the camera so it doesn't appear on in the view but does return a reasonable angle
		// for a circular pointer (like a compass pointing toward the enemy.
		FacingBackward = true;
		float DistanceToCamera = (InLocation - Location).Size(); //Get distance to camera
		ContrivedParallelLocation = InLocation + (ViewPointActor->GetActorForwardVector() * (DistanceToCamera + 10000.f)); //Project forward
//        DrawDebugSphere(
//                        GetWorld(),
//                        ContrivedParallelLocation,
//                        15000,
//                        64,
//                        FColor(255,255,255),false,2.f,0
//                        );
	}
	if (!PlayerController->ProjectWorldLocationToScreen(ContrivedParallelLocation, OutScreenPosition))  GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Projected FALSE"));
	// Check to see if it's on screen. If it is, ProjectWorldLocationToScreen is all we need, return it.
	if ((OutScreenPosition.X >= 0.f && OutScreenPosition.X <= ViewportSize.X
		&& OutScreenPosition.Y >= 0.f && OutScreenPosition.Y <= ViewportSize.Y) && (!FacingBackward))
	{
		bIsOnScreen = true;
		return;
	}
	OutScreenPosition -= ViewportCenter;
	float AngleRadians = FMath::Atan2(OutScreenPosition.Y, OutScreenPosition.X);
	AngleRadians -= FMath::DegreesToRadians(90.f);
	OutRotationAngleDegrees = FMath::RadiansToDegrees(AngleRadians) + 180.f;
	float Cos = cosf(AngleRadians);
	float Sin = -sinf(AngleRadians);
	OutScreenPosition = FVector2D(ViewportCenter.X + (Sin * 180.f), ViewportCenter.Y + Cos * 180.f);
	float m = Cos / Sin;
	FVector2D ScreenBounds = ViewportCenter * EdgePercent;
	if (Cos > 0)
	{
		OutScreenPosition = FVector2D(ScreenBounds.Y / m, ScreenBounds.Y);
	}
	else
	{
		OutScreenPosition = FVector2D(-ScreenBounds.Y / m, -ScreenBounds.Y);
	}
	if (OutScreenPosition.X > ScreenBounds.X)
	{
		OutScreenPosition = FVector2D(ScreenBounds.X, ScreenBounds.X*m);
	}
	else if (OutScreenPosition.X < -ScreenBounds.X)
	{
		OutScreenPosition = FVector2D(-ScreenBounds.X, -ScreenBounds.X*m);
	}
	OutScreenPosition += ViewportCenter;
}

void UCPPMathLib::UUToMeters(const float&InUU, float &OutMeters) {
	OutMeters = InUU / meterPerUU;
}

void UCPPMathLib::UUToKiloMeters(const float &InUU, float &OutKiloMeters) {
	OutKiloMeters = InUU / kmPerUU;
}

void UCPPMathLib::MetersToUU(const float &InMeters, float &OutUU) {
	OutUU = InMeters * meterPerUU;
}

void UCPPMathLib::KiloMetersToUU(const float &InKiloMeters, float &OutUU) {
	OutUU = InKiloMeters * kmPerUU;
}

void UCPPMathLib::NormalDistribution(const float &InMean, const float &InStandardDeaviation, float &OutValue) {
	std::random_device rd{};
	std::mt19937 gen{ rd() };
	std::normal_distribution<float>d{ InMean, InStandardDeaviation };
	OutValue = d(gen);
}

void UCPPMathLib::GetGalaxyPositionPrecise(UObject *WorldContextObject, const FVector &InLocalPosition, FVector &OutGalaxyPosition) {
	FVector KiloMeters;
	UUToKiloMetersVector(InLocalPosition, KiloMeters);
	FIntVector WorldOrigin = UGameplayStatics::GetWorldOriginLocation(WorldContextObject);
	
	float worldOriginXKM, worldOriginYKM, worldOriginZKM;
	UUToKiloMeters((float)WorldOrigin.X, worldOriginXKM);
	UUToKiloMeters((float)WorldOrigin.Y, worldOriginYKM);
	UUToKiloMeters((float)WorldOrigin.Z, worldOriginZKM);

	OutGalaxyPosition.X = KiloMeters.X + worldOriginXKM;
	OutGalaxyPosition.Y = KiloMeters.Y + worldOriginYKM;
	OutGalaxyPosition.Z = KiloMeters.Z + worldOriginZKM;
}

void UCPPMathLib::UUToKiloMetersVector(const FVector &InUU, FVector &OutKiloMeters) {
	UUToKiloMeters(InUU.X, OutKiloMeters.X);
	UUToKiloMeters(InUU.Y, OutKiloMeters.Y);
	UUToKiloMeters(InUU.Z, OutKiloMeters.Z);
}

void UCPPMathLib::KiloMetersToUUVector(const FVector &InKiloMeters, FVector &OutUU) {
	KiloMetersToUU(InKiloMeters.X, OutUU.X);
	KiloMetersToUU(InKiloMeters.Y, OutUU.Y);
	KiloMetersToUU(InKiloMeters.Z, OutUU.Z);
}

void UCPPMathLib::KiloMetersToUUInteger(const int &InKiloMeter, int &OutUU) {
	OutUU = InKiloMeter * (int)kmPerUU;
}

void UCPPMathLib::KiloMetersToUUIntVector(const FIntVector &InKiloMeter, FIntVector &OutUU) {
	KiloMetersToUUInteger(InKiloMeter.X, OutUU.X); 
	KiloMetersToUUInteger(InKiloMeter.Y, OutUU.Y); 
	KiloMetersToUUInteger(InKiloMeter.Z, OutUU.Z);
}

void UCPPMathLib::WorldPositionToLocalOffset(UObject *WorldContextObject, const FVector &InWorldPosition, FVector &OutLocalPosition) {
	FIntVector worldOrigin = UGameplayStatics::GetWorldOriginLocation(WorldContextObject);

	OutLocalPosition.X = InWorldPosition.X - worldOrigin.X;
	OutLocalPosition.Y = InWorldPosition.Y - worldOrigin.Y;
	OutLocalPosition.Z = InWorldPosition.Z - worldOrigin.Z;
}

bool UCPPMathLib::MoveActorToLevel(AActor* InActorToMove, ULevel* InTargetLevel) {
	if (IsValid(InActorToMove)) {
		return InActorToMove->Rename(nullptr, InTargetLevel);
	}
	return false;
}

FVector UCPPMathLib::CalculateTargetLead(FVector InShooterPosition, FVector InShooterVelocity, float InShootSpeed, FVector InTargetPosition, FVector InTargetVelocity) {
	FVector targetRelativePosition = InTargetPosition - InShooterPosition;
	FVector targetRelativeVelocity = InTargetVelocity - InShooterVelocity;
	float t = CalculateTargetLeadTime(InShootSpeed, targetRelativePosition, targetRelativeVelocity);
	return InTargetPosition + t * (targetRelativeVelocity);
}

float UCPPMathLib::CalculateTargetLeadTime(float shootSpeed, FVector targetRelativePosition, FVector targetRelativeVelocity) {
	float velocitySquared = targetRelativeVelocity.SizeSquared();
	if (velocitySquared < 0.001f) {
		return 0.0f;
	}

	float a = velocitySquared - shootSpeed * shootSpeed;

	if (FMath::Abs(a) < 0.001f) {
		float t = -targetRelativePosition.SizeSquared() / (2.0f*FVector::DotProduct(targetRelativeVelocity, targetRelativePosition));
		return FMath::Max(t, 0.0f);
	}

	float b = 2.0f * FVector::DotProduct(targetRelativeVelocity, targetRelativePosition);
	float c = targetRelativePosition.SizeSquared();
	float d = b * b - 4.0f*a*c;

	if (d > 0.0f) {
		float t1 = (-b + FMath::Sqrt(d)) / (2.0f*a);
		float t2 = (-b - FMath::Sqrt(d)) / (2.0f*a);

		if (t1 > 0.0f) {
			if (t2 > 0.0f) {
				return FMath::Min(t1, t2);
			}
			else {
				return t1;
			}
		}
		else {
			return FMath::Max(t2, 0.0f);
		}
	}
	else if (d < 0.0f) {
		return 0.0f;
	}
	else {
		return FMath::Max(-b / (2.0f*a), 0.0f);
	}
}