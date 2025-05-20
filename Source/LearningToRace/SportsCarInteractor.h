// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "LearningAgentsInteractor.h"
#include "LearningAgentsObservations.h"
#include "LearningAgentsActions.h"
#include "LearningToRaceSportsCar.h"
#include "ChaosVehicleMovementComponent.h"
#include "SportsCarInteractor.generated.h"

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

	virtual void SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema) override;

	virtual void GatherAgentObservation_Implementation(FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject, const int32 AgentId) override;

	virtual void SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema) override;

	virtual void PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject, const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId) override;

private:
	UPROPERTY()
	TArray<float> trackDistances = { 0.0f, 500.0f, 1000.0f, 1500.0f, 2000.0f, 2500.0f, 3000.0f }; // Distances to the next track points in centimeters
};
