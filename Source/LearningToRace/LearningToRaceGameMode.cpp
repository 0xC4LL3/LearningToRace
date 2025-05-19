// Copyright Epic Games, Inc. All Rights Reserved.

#include "LearningToRaceGameMode.h"
#include "LearningToRacePlayerController.h"

ALearningToRaceGameMode::ALearningToRaceGameMode()
{
	PlayerControllerClass = ALearningToRacePlayerController::StaticClass();

	LearningAgentsManager = CreateDefaultSubobject<ULearningAgentsManager>("LearningAgentsManager");
}
