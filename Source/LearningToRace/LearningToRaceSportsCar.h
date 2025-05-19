// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LearningToRaceSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class LEARNINGTORACE_API ALearningToRaceSportsCar : public ALearningToRacePawn
{
	GENERATED_BODY()
	
public:

	ALearningToRaceSportsCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
