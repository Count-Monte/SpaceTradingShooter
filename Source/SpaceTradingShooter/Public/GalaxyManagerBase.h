// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine.h"
#include "GalaxyManagerBase.generated.h"

class AStarfieldManagerBase;
class AStellarBodyBase;

UCLASS(BlueprintType)
class SPACETRADINGSHOOTER_API AGalaxyManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AGalaxyManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<FIntVector> StarLocations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<AStellarBodyBase*> activeStars;
	FIntVector PlayerLocation; //Player location in km
	APawn *PlayerPawn;
	bool initialLoad = false;
	int TicksSinceLastUpdate = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StarfieldUpdateRate = 1;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override; 

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void SpawnStar(UPARAM(ref) const FIntVector &Location); 

	UFUNCTION(BlueprintCallable)
	void SpawnStarIfNoneExists(UPARAM(ref)const FIntVector &Location);


	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AStarfieldManagerBase *StarFieldManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int AmountOfStars = 2000;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int GalaxySize = 2000;

};
