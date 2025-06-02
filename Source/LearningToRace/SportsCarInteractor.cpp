// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarInteractor.h"
#include "SportsCarManager.h"

void USportsCarInteractor::SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema)
{
	TMap<FName, FLearningAgentsObservationSchemaElement> SchemaMapping;
	
	FLearningAgentsObservationSchemaElement VelocitySchema = ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema);
	FLearningAgentsObservationSchemaElement TerrainSchema = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema);
	TerrainSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, TerrainSchema, TerrainRaycastCount / TerrainRayDistances.Num());

	SchemaMapping.Add(TEXT("Car"), VelocitySchema);
	SchemaMapping.Add(TEXT("Terrain"), TerrainSchema);

	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);
	
	TerrainFOVAngleOffsets.SetNum(TerrainRaycastCount / TerrainRayDistances.Num());
	for (int i = 0; i < TerrainFOVAngleOffsets.Num(); ++i)
	{
		TerrainFOVAngleOffsets[i] = TerrainFOVStartAngle + i * TerrainFOVStep;
	}
}

void USportsCarInteractor::GatherAgentObservation_Implementation(FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	UObject* AgentObject = LearningAgentsManager->GetAgent(AgentId);
	ALearningToRaceSportsCar* ObservationActor = Cast<ALearningToRaceSportsCar>(AgentObject);
	
	TMap<FName, FLearningAgentsObservationObjectElement> ObjectMapping;
	FVector Location = ObservationActor->GetActorLocation();
	FTransform Transform = ObservationActor->GetActorTransform();
	bool bIsPlayerControlled = ObservationActor->IsPlayerControlled();

	FVector Velocity = ObservationActor->GetVelocity();
	FLearningAgentsObservationObjectElement VelocityObject = ULearningAgentsObservations::MakeVelocityObservation(InObservationObject, Velocity, Transform);

	TArray<FLearningAgentsObservationObjectElement> TerrainObservations;
	FHitResult TerrainHitResult;
	for (int i = 0; i < TerrainFOVAngleOffsets.Num(); ++i)
	{
		FRotator Rotation = ObservationActor->GetActorRotation();
		Rotation.Yaw += TerrainFOVAngleOffsets[i];
		FVector TrackLimit = Location;
		for (int j = 0; j < TerrainRayDistances.Num(); ++j)
		{
			FVector RayEnd = Location + Rotation.Vector() * TerrainRayDistances[j];
			RayEnd.Z = -1.0f;
			GetWorld()->LineTraceSingleByChannel(TerrainHitResult, Location, RayEnd, ECC_Visibility, CollisionParams);
			if (TerrainHitResult.bBlockingHit)
			{
				ALandscapeSplineActor* LandscapeSplineActor = Cast<ALandscapeSplineActor>(TerrainHitResult.HitObjectHandle.FetchActor());
				if (LandscapeSplineActor)
				{
					TrackLimit = TerrainHitResult.ImpactPoint;

					if (j == 0 && ObservationActor->bIsOffroad) { ObservationActor->bIsOffroad = false; }
				}
				else
				{
					if (j == 0 && !ObservationActor->bIsOffroad) { ObservationActor->bIsOffroad = true; }

					break;
				}
			}
		}
		FLearningAgentsObservationObjectElement TerrainObject = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, TrackLimit, Transform, TEXT("LocationObservation"), bIsPlayerControlled, this, AgentId, TrackLimit, FLinearColor::Green);
		TerrainObservations.Add(TerrainObject);

		if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, TrackLimit, FColor::Green); }
	}
	FLearningAgentsObservationObjectElement TerrainObject = ULearningAgentsObservations::MakeStaticArrayObservation(InObservationObject, TerrainObservations);

	ObjectMapping.Add(TEXT("Car"), VelocityObject);
	ObjectMapping.Add(TEXT("Terrain"), TerrainObject);

	OutObservationObjectElement = ULearningAgentsObservations::MakeStructObservation(InObservationObject, ObjectMapping);
}

void USportsCarInteractor::SpecifyAgentAction_Implementation(FLearningAgentsActionSchemaElement& OutActionSchemaElement, ULearningAgentsActionSchema* InActionSchema)
{
	FLearningAgentsActionSchemaElement SteeringSchema = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f, "Steering");
	FLearningAgentsActionSchemaElement ThrottleSchema = ULearningAgentsActions::SpecifyFloatAction(InActionSchema, 1.0f, "ThrottleBrake");

	TMap<FName, FLearningAgentsActionSchemaElement> SchemaMapping;
	SchemaMapping.Add(TEXT("Steering"), SteeringSchema);
	SchemaMapping.Add(TEXT("ThrottleBrake"), ThrottleSchema);

	OutActionSchemaElement = ULearningAgentsActions::SpecifyStructAction(InActionSchema, SchemaMapping);
}

void USportsCarInteractor::PerformAgentAction_Implementation(const ULearningAgentsActionObject* InActionObject, const FLearningAgentsActionObjectElement& InActionObjectElement, const int32 AgentId)
{
	TMap<FName, FLearningAgentsActionObjectElement> OutElements;
	ULearningAgentsActions::GetStructAction(OutElements, InActionObject, InActionObjectElement);

	float SteeringAction;
	float ThrottleAction;
	ULearningAgentsActions::GetFloatAction(SteeringAction, InActionObject, OutElements[TEXT("Steering")], TEXT("Steering"));
	ULearningAgentsActions::GetFloatAction(ThrottleAction, InActionObject, OutElements[TEXT("ThrottleBrake")], TEXT("ThrottleBrake"));

	UObject* AgentObject = LearningAgentsManager->GetAgent(AgentId);
	ALearningToRaceSportsCar* ActionActor = Cast<ALearningToRaceSportsCar>(AgentObject);
	UChaosVehicleMovementComponent* VehicleMovementComponent = ActionActor->GetVehicleMovementComponent();

	VehicleMovementComponent->SetSteeringInput(SteeringAction);
	if (ThrottleAction > 0.0f)
	{
		VehicleMovementComponent->SetThrottleInput(ThrottleAction);
		VehicleMovementComponent->SetBrakeInput(0.0f);
	}
	else
	{
		VehicleMovementComponent->SetBrakeInput(-ThrottleAction);
		VehicleMovementComponent->SetThrottleInput(0.0f);
	}
}