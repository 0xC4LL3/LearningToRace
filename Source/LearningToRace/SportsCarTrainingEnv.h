// Copyright (c) 2025 0xC4LL3. Licensed under the MIT License. See LICENSE file for details.

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
	ULearningAgentsManager* LearningAgentsManager;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	USplineComponent* TrackSpline;

	virtual void GatherAgentReward_Implementation(float& OutReward, const int32 AgentId) override;

	virtual void GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId) override;

	virtual void ResetAgentEpisode_Implementation(const int32 AgentId) override;
};
