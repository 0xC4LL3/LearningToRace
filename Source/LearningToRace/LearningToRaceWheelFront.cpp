// Copyright Epic Games, Inc. All Rights Reserved.

#include "LearningToRaceWheelFront.h"
#include "UObject/ConstructorHelpers.h"

ULearningToRaceWheelFront::ULearningToRaceWheelFront()
{
	AxleType = EAxleType::Front;
	bAffectedBySteering = true;
	MaxSteerAngle = 40.f;
}