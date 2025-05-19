// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "LearningToRaceSportsCar.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "LearningAgentsRewards.h"
#include "LearningAgentsCompletions.h"
#include "SportsCarTrainingEnv.generated.h"

/**
 * 
 */
UCLASS()
class LEARNINGTORACE_API USportsCarTrainingEnv : public ULearningAgentsTrainingEnvironment
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	USplineComponent* TrackSpline;

	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};
