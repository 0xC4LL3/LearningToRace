// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LearningToRaceSportsCar.h"
#include "LearningAgentsPolicy.h"
#include "LearningAgentsCritic.h"
#include "LearningAgentsTrainingEnvironment.h"
#include "LearningAgentsCommunicator.h"
#include "LearningAgentsPPOTrainer.h"
#include "LearningAgentsNeuralNetwork.h"
#include "SportsCarInteractor.h"
#include "SportsCarTrainingEnv.h"
#include "SportsCarManager.generated.h"

UCLASS()
class LEARNINGTORACE_API ASportsCarManager : public AActor
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	ULearningAgentsNeuralNetwork* EncoderNetwork;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	ULearningAgentsNeuralNetwork* PolicyNetwork;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	ULearningAgentsNeuralNetwork* DecoderNetwork;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	ULearningAgentsNeuralNetwork* CriticNetwork;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	FLearningAgentsPolicySettings PolicySettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	FLearningAgentsCriticSettings CriticSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	FLearningAgentsPPOTrainerSettings PPOTrainerSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	FLearningAgentsPPOTrainingSettings PPOTrainingSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	FLearningAgentsTrainingGameSettings TrainingGameSettings;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	bool bShouldRunInference = false;

	// Sets default values for this actor's properties
	ASportsCarManager();

private:
	UPROPERTY()
	TArray<AActor*> CarActors;
	UPROPERTY()
	USplineComponent* TrackSpline;
	UPROPERTY()
	ULearningAgentsInteractor* Interactor;
	UPROPERTY()
	ULearningAgentsPolicy* Policy;
	UPROPERTY()
	ULearningAgentsCritic* Critic;
	UPROPERTY()
	ULearningAgentsTrainingEnvironment* Environment;
	UPROPERTY()
	FLearningAgentsTrainerProcess TrainerProcess;
	UPROPERTY()
	FLearningAgentsCommunicator Communicator;
	UPROPERTY()
	ULearningAgentsPPOTrainer* PPOTrainer;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
