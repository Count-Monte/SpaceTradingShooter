// Fill out your copyright notice in the Description page of Project Settings.


#include "CompleteWorldOriginShifter.h"
#include "Engine.h"

// Sets default values
ACompleteWorldOriginShifter::ACompleteWorldOriginShifter()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACompleteWorldOriginShifter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACompleteWorldOriginShifter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!IsValid(PlayerPawn)) {
		APawn *pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (IsValid(pawn)) {
			PlayerPawn = pawn;
		}
		else {
			GEngine->AddOnScreenDebugMessage(1000, 20.0f, FColor::Red, TEXT("No active player pawn in level failed to check world origin shift."));
			return;
		}
	}

	FIntVector worldOrigin = GetWorld()->OriginLocation;
	FVector playerLoc = PlayerPawn->GetActorLocation() + FVector(worldOrigin);
	worldOrigin /= worldOriginShiftThreshold;
	playerLoc /= worldOriginShiftThreshold;
	FIntVector iPlayerLoc(playerLoc);
	if (worldOrigin != iPlayerLoc) {
		GetWorld()->SetNewWorldOrigin(iPlayerLoc * worldOriginShiftThreshold);
	}
	GEngine->AddOnScreenDebugMessage(999, 20.0f, FColor::Blue, FString::Printf(TEXT("%s"), *iPlayerLoc.ToString()));
	GEngine->AddOnScreenDebugMessage(998, 20.0f, FColor::Blue, FString::Printf(TEXT("%s"), *playerLoc.ToString()));
	worldOrigin = GetWorld()->OriginLocation;
	GEngine->AddOnScreenDebugMessage(997, 20.0f, FColor::Blue, FString::Printf(TEXT("%s"), *worldOrigin.ToString()));
}

