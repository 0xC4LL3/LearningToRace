// Copyright Epic Games, Inc. All Rights Reserved.

#include "LearningToRaceWheelRear.h"
#include "UObject/ConstructorHelpers.h"

ULearningToRaceWheelRear::ULearningToRaceWheelRear()
{
	AxleType = EAxleType::Rear;
	bAffectedByHandbrake = true;
	bAffectedByEngine = true;
}