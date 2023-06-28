// Fill out your copyright notice in the Description page of Project Settings.


#include "StarfieldManagerBase.h"
#include "ConstructorHelpers.h"
#include "Engine.h"
#include "Quat.h"
#include "CPPMathLib.h"
#include "Engine/Classes/Kismet/KismetMathLibrary.h"
#include "GalaxyManagerBase.h"

#define print(text) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White,text);

// Sets default values
AStarfieldManagerBase::AStarfieldManagerBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	USceneComponent *SceneComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));
	RootComponent = SceneComponent;
	
	Starfield = CreateDefaultSubobject<UHierarchicalInstancedStaticMeshComponent>(TEXT("Starfield"));
	Starfield->SetupAttachment(RootComponent);
	static ConstructorHelpers::FObjectFinder<UStaticMesh> BillboardMesh(TEXT("/Game/Materials/StarBillboard_Mesh.StarBillboard_Mesh"));
	if(BillboardMesh.Succeeded()) {
		Starfield->SetStaticMesh(BillboardMesh.Object);
	}
}

// Called when the game starts or when spawned
void AStarfieldManagerBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AStarfieldManagerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!IsValid(PlayerPawn)) {
		APawn *pawn = UGameplayStatics::GetPlayerPawn(this, 0);
		if (IsValid(pawn)) {
			PlayerPawn = pawn;
			AttachToActor(PlayerPawn, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
		}
		else {
			return;
		}
	}
	//SetActorLocation(PlayerPawn->GetActorLocation());
	FRotator rot(0.0f, 0.0f, 0.0f);
	SetActorRotation(rot);
}

void AStarfieldManagerBase::UpdateStarBillboards(TArray<FIntVector> &StarLocations, FVector &LocalLocation) {
	Starfield->ClearInstances();
	if (!IsValid(PlayerPawn)) return;

	FVector preciseGalaxyPosition;
	UCPPMathLib::GetGalaxyPositionPrecise(this, LocalLocation, preciseGalaxyPosition);

	float StarFarViewRangeSqr = pow(StarFarViewRange, 2.0f);
	float StarNearViewRangeSqr = pow(StarNearViewRange, 2.0f);
	FVector playerForward = PlayerPawn->GetActorForwardVector();
	GEngine->AddOnScreenDebugMessage(3, 0.0f, FColor::Yellow, FString::Printf(TEXT("%s"), *playerForward.ToString()));

	for (FIntVector StarLocation : StarLocations) {
		FVector FStarLocation = FVector(StarLocation);
		FVector starDirectionVector = UKismetMathLibrary::GetDirectionUnitVector(preciseGalaxyPosition, FStarLocation);
		//print(FString::Printf(TEXT("%s"), *starDirectionVector.ToString()))

		float starAngleToPlayerForward = FVector::DotProduct(starDirectionVector, playerForward);
		starAngleToPlayerForward = 1.0f;
		if (!(starAngleToPlayerForward >= 0.2f)) {
			continue;
		}

		float starDistanceSqr = FVector::DistSquared(FStarLocation, preciseGalaxyPosition);
		float scalingFactor = 1.0f - UKismetMathLibrary::NormalizeToRange(starDistanceSqr, StarFarViewRangeSqr, StarNearViewRangeSqr);
		if (scalingFactor >= 1.0f) {
			// TODO Spawn solar system
			GalaxyManager->SpawnStarIfNoneExists(StarLocation);
		}

		if (!UKismetMathLibrary::InRange_FloatFloat(scalingFactor, 0.0f, 1.0f)) {
			continue;
		}
		// Create star billboard
		FVector starScale = FVector(scalingFactor * ClosestStarScale);
		FVector StarRenderPosition = starDirectionVector * StarRenderOffset;
		FQuat starRotation = FQuat(UKismetMathLibrary::MakeRotFromX(starDirectionVector))*FQuat::MakeFromEuler(FVector(0.0f, 90.0f, 0.0f));
		FTransform transform(starRotation, StarRenderPosition, starScale);
		Starfield->AddInstance(transform);
	}
}

