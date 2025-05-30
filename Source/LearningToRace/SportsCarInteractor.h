// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LandscapeSplineActor.h"
#include "LandscapeSplineSegment.h"
#include "Components/SplineMeshComponent.h"
#include "Components/SplineComponent.h"
#include "Engine/World.h"
#include "Kismet/KismetSystemLibrary.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsObservations.h"
#include "LearningAgentsActions.h"
#include "LearningToRaceSportsCar.h"
#include "ChaosVehicleMovementComponent.h"
#include "SportsCarInteractor.generated.h"

class ASportsCarManager;

/**
 * 
 */
UCLASS()
class LEARNINGTORACE_API USportsCarInteractor : public ULearningAgentsInteractor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	USplineComponent* TrackSpline;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	ASportsCarManager* SportsCarManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	ULearningAgentsManager* LearningAgentsManager;

	FCollisionQueryParams CollisionParams;

	virtual void SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema) override;

	virtual void GatherAgentObservation_Implementation(FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject, const int32 AgentId) override;

	virtual void SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema) override;

	virtual void PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject, const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId) override;

private:
	UPROPERTY()
	TArray<float> TrackDistances = { 0.0f, 500.0f, 1000.0f, 1500.0f, 2000.0f, 2500.0f, 3000.0f }; // Distances to the next track points in centimeters

	static const int CollisionRaycastCount = 11;
	const float CollisionRayLength = 3000.0f;
	const float CollisionFieldOfView = 120.0f;
	const float CollisionFOVStartAngle = -CollisionFieldOfView / 2.0f;
	const float CollisionFOVStep = CollisionFieldOfView / (CollisionRaycastCount - 1);
	TArray<float> CollisionFOVOffsets;

	static const int TerrainRaycastCount = 600;
	const TArray<float> TerrainRayDistances = { 100.0f, 200.0f, 300.0f, 400.0f, 500.0f, 600.0f, 700.0f, 800.0f, 900.0f, 1000.0f, 1100.0f, 1200.0f, 1300.0f, 1400.0f, 1500.0f, 1600.0f, 1700.0f, 1800.0f, 1900.0f, 2000.0f, 2100.0f, 2200.0f, 2300.0f, 2400.0f, 2500.0f, 2600.0f, 2700.0f, 2800.0f, 2900.0f, 3000.0f };
	const float TerrainFieldOfView = 120.0f;
	const float TerrainFOVStartAngle = -TerrainFieldOfView / 2.0f;
	const float TerrainFOVStep = TerrainFieldOfView / (TerrainRaycastCount / 30 - 1);
	TArray<float> TerrainFOVAngleOffsets;
};
