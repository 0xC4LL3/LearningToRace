// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LearningAgentsManager.h"
#include "LearningToRaceGameMode.generated.h"

UCLASS(MinimalAPI)
class ALearningToRaceGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Learning")
	ULearningAgentsManager* LearningAgentsManager;

	ALearningToRaceGameMode();
};



