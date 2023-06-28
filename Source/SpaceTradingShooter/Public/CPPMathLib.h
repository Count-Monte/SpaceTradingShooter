// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "CPPMathLib.generated.h"

/**
 * 
 */
UCLASS()
class SPACETRADINGSHOOTER_API UCPPMathLib : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
private:
	static float cmPerUU;
	static float meterPerUU;
	static float kmPerUU;
public:
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject", CallableWithoutWorldContext), Category="HUD|Util")
		static void FindScreenEdgeLocationForWorldLocation(UObject *WorldContextObject, const FVector& InLocation, const float EdgePercent, FVector2D& OutScreenPosition, float& OutRotationAngleDegrees, bool &bIsOnScreen, bool &FacingBackward);

	UFUNCTION(BlueprintPure, Category = "Math|Conversions")
		static void UUToMeters(const float &InUU, float &OutMeters); 
	UFUNCTION(BlueprintPure, Category = "Math|Conversions")
		static void UUToKiloMeters(const float &InUU, float &OutKiloMeters);
	UFUNCTION(BlueprintPure, Category = "Math|Conversions")
		static void MetersToUU(const float &InMeters, float &OutUU);
	UFUNCTION(BlueprintPure, Category = "Math|Conversions")
		static void KiloMetersToUU(const float &InKiloMeters, float &OutUU);
	UFUNCTION(BlueprintPure, Category = "Math|Float")
		static void NormalDistribution(const float &InMean, const float &InStandardDeaviation, float &OutValue);
	UFUNCTION(BlueprintPure, meta=(WorldContext="WorldContextObject"), Category = "Math")
		static void GetGalaxyPositionPrecise(UObject *WorldContextObject, const FVector &InLocalPosition, FVector &OutGalaxyPosition);
	UFUNCTION(BlueprintPure, Category = "Math|Vector")
		static void UUToKiloMetersVector(const FVector &InUU, FVector &OutKilometers); 
	UFUNCTION(BlueprintPure, Category = "Math|Vector")
		static void KiloMetersToUUVector(const FVector &InKiloMeters, FVector &OutUU);
	UFUNCTION(BlueprintPure, Category = "Math|Integer")
		static void KiloMetersToUUInteger(const int &InKiloMeter, int &OutUU);
	UFUNCTION(BlueprintPure, Category = "Math|IntVector")
		static void KiloMetersToUUIntVector(const FIntVector &InKiloMeter, FIntVector &OutUU);
	UFUNCTION(BlueprintPure, meta = (WorldContext = "WorldContextObject"), Category = "Math|Conversions")
		static void WorldPositionToLocalOffset(UObject *WorldContextObject, const FVector &InWorldPosition, FVector &OutLocalOffset);
	UFUNCTION(BlueprintPure)
		static bool MoveActorToLevel(AActor* InActorToMove, ULevel* InTargetLevel);
	UFUNCTION(BlueprintPure)
		static FVector CalculateTargetLead(FVector InShooterPosition, FVector InShooterVelocity, float InShootSpeed, FVector InTargetPosition, FVector InTargetVelocity);
	static float CalculateTargetLeadTime(float shootSpeed, FVector targetRelativePosition, FVector targetRelativeVelocity);

};
