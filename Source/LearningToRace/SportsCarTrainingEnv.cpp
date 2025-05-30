// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarTrainingEnv.h"

void USportsCarTrainingEnv::GatherAgentReward_Implementation(float& OutReward, const int32 AgentId)
{
	UObject* AgentObject = LearningAgentsManager->GetAgent(AgentId);
	ALearningToRaceSportsCar* RewardActor = Cast<ALearningToRaceSportsCar>(AgentObject);

	FVector ActorLocation = RewardActor->GetActorLocation();
	FVector ActorVelocity = RewardActor->GetVelocity();
	FVector SplineLocation = TrackSpline->FindLocationClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World);

	//float LocationReward = ULearningAgentsRewards::MakeRewardOnLocationDifferenceAboveThreshold(ActorLocation, SplineLocation, 800.0f, -10.0f);
	//float LocationReward = ULearningAgentsRewards::MakeReward(1.0f, RewardActor->bIsOffroad ? -10.0f : 1.0f);
	float VelocityReward = ULearningAgentsRewards::MakeRewardFromVelocityAlongSpline(TrackSpline, ActorLocation, ActorVelocity, 1000.0f, 1.0f, 10.0f);

	OutReward = VelocityReward; // LocationReward + 
}

void USportsCarTrainingEnv::GatherAgentCompletion_Implementation(ELearningAgentsCompletion& OutCompletion, const int32 AgentId)
{
	UObject* AgentObject = LearningAgentsManager->GetAgent(AgentId);
	ALearningToRaceSportsCar* RewardActor = Cast<ALearningToRaceSportsCar>(AgentObject);

	if (RewardActor->bHasCrashed)
	{
		OutCompletion = ULearningAgentsCompletions::MakeCompletion(ELearningAgentsCompletion::Termination);
		return;
	}

	OutCompletion = ULearningAgentsCompletions::MakeCompletionOnCondition(RewardActor->bIsOffroad, ELearningAgentsCompletion::Termination);

	//FVector ActorLocation = RewardActor->GetActorLocation();
	//FVector SplineLocation = TrackSpline->FindLocationClosestToWorldLocation(ActorLocation, ESplineCoordinateSpace::World);

	//OutCompletion = ULearningAgentsCompletions::MakeCompletionOnLocationDifferenceAboveThreshold(SplineLocation, ActorLocation, 800.0f, ELearningAgentsCompletion::Termination);
}

void USportsCarTrainingEnv::ResetAgentEpisode_Implementation(const int32 AgentId)
{
	ALearningToRaceSportsCar* AgentActor = Cast<ALearningToRaceSportsCar>(LearningAgentsManager->GetAgent(AgentId));
	AgentActor->ResetToRandomPointOnSpline(TrackSpline);
}