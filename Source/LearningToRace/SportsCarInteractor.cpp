// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarInteractor.h"

void USportsCarInteractor::SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema)
{
	FLearningAgentsObservationSchemaElement LocationSchema = ULearningAgentsObservations::SpecifyLocationAlongSplineObservation(InObservationSchema);
	FLearningAgentsObservationSchemaElement DirectionSchema = ULearningAgentsObservations::SpecifyDirectionAlongSplineObservation(InObservationSchema);

	TMap<FName, FLearningAgentsObservationSchemaElement> SchemaMapping;
	SchemaMapping.Add(TEXT("Location"), LocationSchema);
	SchemaMapping.Add(TEXT("Direction"), DirectionSchema);

	FLearningAgentsObservationSchemaElement TrackSchema = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);
	TrackSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, TrackSchema, trackDistances.Num());
	
	FLearningAgentsObservationSchemaElement VelocitySchema = ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema);

	SchemaMapping.Empty();
	SchemaMapping.Add(TEXT("Track"), TrackSchema);
	SchemaMapping.Add(TEXT("Car"), VelocitySchema);

	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);
}

void USportsCarInteractor::GatherAgentObservation_Implementation(FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	UObject* AgentObject = Cast<ALearningToRaceGameMode>(UGameplayStatics::GetGameMode(this))->LearningAgentsManager->GetAgent(AgentId);
	ALearningToRaceSportsCar* ObservationActor = Cast<ALearningToRaceSportsCar>(AgentObject);
	
	TArray<FLearningAgentsObservationObjectElement> trackObservations;
	TMap<FName, FLearningAgentsObservationObjectElement> ObjectMapping;
	FVector Location = ObservationActor->GetActorLocation();
	FTransform Transform = ObservationActor->GetActorTransform();
	float distanceAlongSpline = TrackSpline->GetDistanceAlongSplineAtLocation(Location, ESplineCoordinateSpace::World);
	bool isPlayerControlled = ObservationActor->IsPlayerControlled();
	for(float trackDistance : trackDistances)
	{
		float sampleDistance = distanceAlongSpline + trackDistance;
		FVector locationAtDistanceAlongSpline = TrackSpline->GetLocationAtDistanceAlongSpline(sampleDistance, ESplineCoordinateSpace::World);
		
		FLearningAgentsObservationObjectElement LocationObject = ULearningAgentsObservations::MakeLocationAlongSplineObservation(InObservationObject, TrackSpline, sampleDistance, Transform, TEXT("LocationAlongSplineObservation"), isPlayerControlled, this, AgentId, locationAtDistanceAlongSpline, FLinearColor::Blue);
		FLearningAgentsObservationObjectElement DirectionObject = ULearningAgentsObservations::MakeDirectionAlongSplineObservation(InObservationObject, TrackSpline, sampleDistance, Transform, TEXT("DirectionAlongSplineObservation"), isPlayerControlled, this, AgentId, locationAtDistanceAlongSpline, 100.0f, FLinearColor::Yellow);

		ObjectMapping.Add(TEXT("Location"), LocationObject);
		ObjectMapping.Add(TEXT("Direction"), DirectionObject);

		FLearningAgentsObservationObjectElement TrackObject = ULearningAgentsObservations::MakeStructObservation(InObservationObject, ObjectMapping);
		trackObservations.Add(TrackObject);
	}
	FLearningAgentsObservationObjectElement TrackObject = ULearningAgentsObservations::MakeStaticArrayObservation(InObservationObject, trackObservations);

	FVector Velocity = ObservationActor->GetVelocity();
	FLearningAgentsObservationObjectElement VelocityObject = ULearningAgentsObservations::MakeVelocityObservation(InObservationObject, Velocity, Transform);

	ObjectMapping.Empty();
	ObjectMapping.Add(TEXT("Track"), TrackObject);
	ObjectMapping.Add(TEXT("Car"), VelocityObject);

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

	UObject* AgentObject = Cast<ALearningToRaceGameMode>(UGameplayStatics::GetGameMode(this))->LearningAgentsManager->GetAgent(AgentId);
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