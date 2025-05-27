// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarInteractor.h"
#include "SportsCarManager.h"

void USportsCarInteractor::SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema)
{
	FLearningAgentsObservationSchemaElement LocationSchema = ULearningAgentsObservations::SpecifyLocationAlongSplineObservation(InObservationSchema);
	FLearningAgentsObservationSchemaElement DirectionSchema = ULearningAgentsObservations::SpecifyDirectionAlongSplineObservation(InObservationSchema);

	TMap<FName, FLearningAgentsObservationSchemaElement> SchemaMapping;
	SchemaMapping.Add(TEXT("Location"), LocationSchema);
	SchemaMapping.Add(TEXT("Direction"), DirectionSchema);

	FLearningAgentsObservationSchemaElement TrackSchema = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);
	TrackSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, TrackSchema, TrackDistances.Num());
	
	FLearningAgentsObservationSchemaElement VelocitySchema = ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema);
	FLearningAgentsObservationSchemaElement VisionSchema = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema);
	VisionSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, VisionSchema, RaycastCount);

	SchemaMapping.Empty();
	SchemaMapping.Add(TEXT("Track"), TrackSchema);
	SchemaMapping.Add(TEXT("Car"), VelocitySchema);
	SchemaMapping.Add(TEXT("Vision"), VisionSchema);

	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);

	FOVOffsets.SetNum(RaycastCount);
	for (int i = 0; i < RaycastCount; ++i)
	{
		// Pre-calculate the relative yaw offset in degrees
		FOVOffsets[i] = FOVStartAngle + i * FOVStep;
	}
}

void USportsCarInteractor::GatherAgentObservation_Implementation(FLearningAgentsObservationObjectElement& OutObservationObjectElement, ULearningAgentsObservationObject* InObservationObject, const int32 AgentId)
{
	UObject* AgentObject = LearningAgentsManager->GetAgent(AgentId);
	ALearningToRaceSportsCar* ObservationActor = Cast<ALearningToRaceSportsCar>(AgentObject);
	
	TArray<FLearningAgentsObservationObjectElement> TrackObservations;
	TMap<FName, FLearningAgentsObservationObjectElement> ObjectMapping;
	FVector Location = ObservationActor->GetActorLocation();
	FTransform Transform = ObservationActor->GetActorTransform();
	float DistanceAlongSpline = TrackSpline->GetDistanceAlongSplineAtLocation(Location, ESplineCoordinateSpace::World);
	bool bIsPlayerControlled = ObservationActor->IsPlayerControlled();
	for(float TrackDistance : TrackDistances)
	{
		float SampleDistance = DistanceAlongSpline + TrackDistance;
		FVector LocationAtDistanceAlongSpline = TrackSpline->GetLocationAtDistanceAlongSpline(SampleDistance, ESplineCoordinateSpace::World);
		
		FLearningAgentsObservationObjectElement LocationObject = ULearningAgentsObservations::MakeLocationAlongSplineObservation(InObservationObject, TrackSpline, SampleDistance, Transform, TEXT("LocationAlongSplineObservation"), bIsPlayerControlled, this, AgentId, LocationAtDistanceAlongSpline, FLinearColor::Blue);
		FLearningAgentsObservationObjectElement DirectionObject = ULearningAgentsObservations::MakeDirectionAlongSplineObservation(InObservationObject, TrackSpline, SampleDistance, Transform, TEXT("DirectionAlongSplineObservation"), bIsPlayerControlled, this, AgentId, LocationAtDistanceAlongSpline, 100.0f, FLinearColor::Yellow);

		ObjectMapping.Add(TEXT("Location"), LocationObject);
		ObjectMapping.Add(TEXT("Direction"), DirectionObject);

		FLearningAgentsObservationObjectElement TrackObject = ULearningAgentsObservations::MakeStructObservation(InObservationObject, ObjectMapping);
		TrackObservations.Add(TrackObject);
	}
	FLearningAgentsObservationObjectElement TrackObject = ULearningAgentsObservations::MakeStaticArrayObservation(InObservationObject, TrackObservations);

	FVector Velocity = ObservationActor->GetVelocity();
	FLearningAgentsObservationObjectElement VelocityObject = ULearningAgentsObservations::MakeVelocityObservation(InObservationObject, Velocity, Transform);

	TArray<FLearningAgentsObservationObjectElement> VisionObservations;
	FHitResult HitResult[RaycastCount];
	for (int i = 0; i < RaycastCount; ++i)
	{
		// Compute the new ray direction based on the rotated vector and use it to get RayEnd
		FRotator Rotation = ObservationActor->GetActorRotation();
		Rotation.Yaw += FOVOffsets[i];
		FVector RayDir = Rotation.Vector();
		RayDir.Z = FMath::Max(0.0f, RayDir.Z); // Prevent looking into the ground while breaking
		FVector RayEnd = Location + RayDir * RayLength;

		// Perform the line trace
		GetWorld()->LineTraceSingleByChannel(HitResult[i], Location, RayEnd, ECC_Visibility, CollisionParams);

		if (HitResult[i].bBlockingHit)
		{
			FLearningAgentsObservationObjectElement VisionObject = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, HitResult[i].ImpactPoint, Transform, TEXT("LocationObservation"), bIsPlayerControlled, this, AgentId, HitResult[i].ImpactPoint, FLinearColor::Green);
			VisionObservations.Add(VisionObject);

			if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, HitResult[i].ImpactPoint, FColor::Red); }
		}
		else
		{
			// If no hit, use the ray end point as the observation
			FLearningAgentsObservationObjectElement VisionObject = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, RayEnd, Transform, TEXT("LocationObservation"), bIsPlayerControlled, this, AgentId, RayEnd, FLinearColor::Green);
			VisionObservations.Add(VisionObject);

			if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, RayEnd, FColor::Red); }
		}
	}
	FLearningAgentsObservationObjectElement VisionObject = ULearningAgentsObservations::MakeStaticArrayObservation(InObservationObject, VisionObservations);

	ObjectMapping.Empty();
	ObjectMapping.Add(TEXT("Track"), TrackObject);
	ObjectMapping.Add(TEXT("Car"), VelocityObject);
	ObjectMapping.Add(TEXT("Vision"), VisionObject);

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