// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "StellarBodyBase.generated.h"

UCLASS(BlueprintType)
class SPACETRADINGSHOOTER_API AStellarBodyBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStellarBodyBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 
	UFUNCTION(BlueprintCallable)
	float GetScalingDistance(UPARAM(ref) FVector &relativeTo);
	UFUNCTION(BlueprintCallable)
		float GetScalingDistanceAU(UPARAM(ref) FVector &relativeToAU);
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(ExposeOnSpawn="true"))
	FVector StellarBodyLocation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MaxVisibleDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MinVisibleDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		bool CalculateVisibleDistances = true;

	/** Steller body's diameter in kilometers */
	UPROPERTY(EditAnywhere, BlueprintReadWrite,  meta=(ExposeOnSpawn="true"))
	float Size = 1.0;

};
