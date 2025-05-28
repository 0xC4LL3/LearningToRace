// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarManager.h"

// Sets default values
ASportsCarManager::ASportsCarManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ASportsCarManager::BeginPlay()
{
	Super::BeginPlay();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ALearningToRaceSportsCar::StaticClass(), CarActors);
	for (AActor* Car : CarActors)
	{
		AddTickPrerequisiteActor(Car);
	}

	TArray<AActor*> OutTracks;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("TrackSpline"), OutTracks);
	TrackSpline = Cast<USplineComponent>(OutTracks[0]->GetComponentByClass(USplineComponent::StaticClass()));

    ULearningAgentsManager* Manager = Cast<ALearningToRaceGameMode>(UGameplayStatics::GetGameMode(this))->LearningAgentsManager;
	Interactor = ULearningAgentsInteractor::MakeInteractor(Manager, USportsCarInteractor::StaticClass(), TEXT("SportsCarInteractor"));
	Cast<USportsCarInteractor>(Interactor)->SportsCarManager = this;
	Cast<USportsCarInteractor>(Interactor)->LearningAgentsManager = Manager;
	Cast<USportsCarInteractor>(Interactor)->TrackSpline = TrackSpline;
	Cast<USportsCarInteractor>(Interactor)->CollisionParams = FCollisionQueryParams::DefaultQueryParam;
	Cast<USportsCarInteractor>(Interactor)->CollisionParams.AddIgnoredActors(CarActors);

	Policy = ULearningAgentsPolicy::MakePolicy(Manager, Interactor, ULearningAgentsPolicy::StaticClass(), TEXT("SportsCarPolicy"), EncoderNetwork, PolicyNetwork, DecoderNetwork, !bShouldRunInference, !bShouldRunInference, !bShouldRunInference, PolicySettings);
	if (bShouldRunInference)
	{
		Policy->GetEncoderNetworkAsset()->LoadNetworkFromSnapshot(EncoderSnapshot);
		Policy->GetPolicyNetworkAsset()->LoadNetworkFromSnapshot(PolicySnapshot);
		Policy->GetDecoderNetworkAsset()->LoadNetworkFromSnapshot(DecoderSnapshot);
	}

	Critic = ULearningAgentsCritic::MakeCritic(Manager, Interactor, Policy, ULearningAgentsCritic::StaticClass(), TEXT("SportsCarCritic"), CriticNetwork, !bShouldRunInference, CriticSettings);
	Environment = ULearningAgentsTrainingEnvironment::MakeTrainingEnvironment(Manager, USportsCarTrainingEnv::StaticClass());
	Cast<USportsCarTrainingEnv>(Environment)->LearningAgentsManager = Manager;
	Cast<USportsCarTrainingEnv>(Environment)->TrackSpline = TrackSpline;

	TrainerProcess = ULearningAgentsCommunicatorLibrary::SpawnSharedMemoryTrainingProcess(TrainerProcessSettings, SharedMemoryCommunicatorSettings);
	Communicator = ULearningAgentsCommunicatorLibrary::MakeSharedMemoryCommunicator(TrainerProcess, SharedMemoryCommunicatorSettings);
	PPOTrainer = ULearningAgentsPPOTrainer::MakePPOTrainer(Manager, Interactor, Environment, Policy, Critic, Communicator, ULearningAgentsPPOTrainer::StaticClass(), TEXT("SportsCarPPOTrainer"), PPOTrainerSettings);

	if (!bShouldRunInference)
	{
		return;
	}

	for (AActor* Car : CarActors)
	{
		Cast<ALearningToRaceSportsCar>(Car)->CarActors = CarActors;
		Cast<ALearningToRaceSportsCar>(Car)->ResetToRandomPointOnSpline(TrackSpline);
	}
}

// Called every frame
void ASportsCarManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bShouldRunInference ? Policy->RunInference(0.0f) : PPOTrainer->RunTraining(PPOTrainingSettings, TrainingGameSettings);
}
