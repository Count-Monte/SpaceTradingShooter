// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Engine/Classes/Components/HierarchicalInstancedStaticMeshComponent.h"
#include "Engine.h"
#include "StarfieldManagerBase.generated.h"

class AGalaxyManagerBase;

UCLASS(BlueprintType)
class SPACETRADINGSHOOTER_API AStarfieldManagerBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AStarfieldManagerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	APawn *PlayerPawn;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void UpdateStarBillboards(UPARAM(ref) TArray<FIntVector> &StarLocations, UPARAM(ref) FVector &LocalLocation);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StarRenderOffset = 4000000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StarFarViewRange = 1500.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float StarNearViewRange = 40.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float ClosestStarScale = 300.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UHierarchicalInstancedStaticMeshComponent *Starfield;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	AGalaxyManagerBase *GalaxyManager;
};
