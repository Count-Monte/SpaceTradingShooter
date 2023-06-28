// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CompleteWorldOriginShifter.generated.h"

UCLASS()
class SPACETRADINGSHOOTER_API ACompleteWorldOriginShifter : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACompleteWorldOriginShifter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APawn *PlayerPawn;
	float worldOriginShiftThreshold = 100000.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
