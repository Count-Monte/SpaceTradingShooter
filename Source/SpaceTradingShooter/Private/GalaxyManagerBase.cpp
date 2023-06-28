// Fill out your copyright notice in the Description page of Project Settings.


#include "GalaxyManagerBase.h"
#include "StarfieldManagerBase.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "CPPMathLib.h"
#include "StellarBodyBase.h"

// Sets default values
AGalaxyManagerBase::AGalaxyManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGalaxyManagerBase::BeginPlay()
{
	Super::BeginPlay();
	for (int i = 0; i < AmountOfStars; i++) {
		FIntVector StarLocation;

		StarLocation.X = UKismetMathLibrary::RandomIntegerInRange(-GalaxySize, GalaxySize);
		StarLocation.Y = UKismetMathLibrary::RandomIntegerInRange(-GalaxySize, GalaxySize);
		StarLocation.Z = UKismetMathLibrary::RandomIntegerInRange(-GalaxySize, GalaxySize);

		StarLocations.AddUnique(StarLocation);
	}

	// Create empty object and runs constructor but waits with running BeginPlayer
	FTransform spawnTransform;
	StarFieldManager = GetWorld()->SpawnActorDeferred<AStarfieldManagerBase>(AStarfieldManagerBase::StaticClass(), spawnTransform);
	StarFieldManager->GalaxyManager = this;
	StarFieldManager->FinishSpawning(spawnTransform);
}

// Called every frame
void AGalaxyManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Make sure theres a valid player pawn 
	if (!IsValid(PlayerPawn)) {
		APawn *pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (IsValid(pawn)) {
			PlayerPawn = pawn;
		}
		else {
			return;
		}
	}

	FVector galaxyPos;
	FVector localLocation = PlayerPawn->GetActorLocation();
	UCPPMathLib::GetGalaxyPositionPrecise(this, localLocation, galaxyPos);
	FIntVector iGalaxyPos(galaxyPos);
	if (iGalaxyPos != PlayerLocation || !initialLoad) {
		FIntVector worldOrigin;
		UCPPMathLib::KiloMetersToUUIntVector(iGalaxyPos, worldOrigin);

		GetWorld()->SetNewWorldOrigin(worldOrigin);
		PlayerLocation = iGalaxyPos;
		if (!initialLoad) {
			FVector temp = FVector(0.0f, 0.0f, 0.0f);
			StarFieldManager->UpdateStarBillboards(StarLocations, temp);
		}
		initialLoad = true;
	}
	if (!(TicksSinceLastUpdate >= StarfieldUpdateRate)) {
		TicksSinceLastUpdate++;
	}
	TicksSinceLastUpdate = 0;
}

void AGalaxyManagerBase::SpawnStar_Implementation(const FIntVector &Location) {

}

void AGalaxyManagerBase::SpawnStarIfNoneExists(const FIntVector &Location) {
	bool foundStar = false;
	for (AStellarBodyBase *star : activeStars) {
		/*if (Location == star->StellarBodyLocation) {
			foundStar = true;
			break;
		}*/
	}
	if (!foundStar) {
		SpawnStar(Location);
	}
}