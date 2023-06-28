// Fill out your copyright notice in the Description page of Project Settings.


#include "StellarBodyBase.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"

// Sets default values
AStellarBodyBase::AStellarBodyBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AStellarBodyBase::BeginPlay()
{
	Super::BeginPlay();
	if (!CalculateVisibleDistances) return;
	MinVisibleDistance = 200.0f;
	MaxVisibleDistance = 30000.0f;
}

// Called every frame
void AStellarBodyBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

float AStellarBodyBase::GetScalingDistance(FVector &relativeTo) {
	FVector myPosition = GetActorLocation();
	float distSqr = UKismetMathLibrary::Vector_DistanceSquared(myPosition, relativeTo);
	float range = 1.0f - UKismetMathLibrary::NormalizeToRange(distSqr, MaxVisibleDistance*MaxVisibleDistance, MinVisibleDistance*MinVisibleDistance);
	return UKismetMathLibrary::FClamp(range, 0.0f, 1.0f);
}

float AStellarBodyBase::GetScalingDistanceAU(FVector &relativeToAU) {
	FVector myPosition = GetActorLocation();
	return 0;
}

