// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarInteractor.h"
#include "SportsCarManager.h"

void USportsCarInteractor::SpecifyAgentObservation_Implementation(FLearningAgentsObservationSchemaElement& OutObservationSchemaElement, ULearningAgentsObservationSchema* InObservationSchema)
{
	//FLearningAgentsObservationSchemaElement LocationSchema = ULearningAgentsObservations::SpecifyLocationAlongSplineObservation(InObservationSchema);
	//FLearningAgentsObservationSchemaElement DirectionSchema = ULearningAgentsObservations::SpecifyDirectionAlongSplineObservation(InObservationSchema);

	TMap<FName, FLearningAgentsObservationSchemaElement> SchemaMapping;
	//SchemaMapping.Add(TEXT("Location"), LocationSchema);
	//SchemaMapping.Add(TEXT("Direction"), DirectionSchema);

	//FLearningAgentsObservationSchemaElement TrackSchema = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);
	//TrackSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, TrackSchema, TrackDistances.Num());
	
	FLearningAgentsObservationSchemaElement VelocitySchema = ULearningAgentsObservations::SpecifyVelocityObservation(InObservationSchema);
	//FLearningAgentsObservationSchemaElement CollisionSchema = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema);
	FLearningAgentsObservationSchemaElement TerrainSchema = ULearningAgentsObservations::SpecifyLocationObservation(InObservationSchema);
	//CollisionSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, CollisionSchema, CollisionRaycastCount);
	TerrainSchema = ULearningAgentsObservations::SpecifyStaticArrayObservation(InObservationSchema, TerrainSchema, TerrainRaycastCount / TerrainRayDistances.Num());

	//SchemaMapping.Empty();
	//SchemaMapping.Add(TEXT("Track"), TrackSchema);
	SchemaMapping.Add(TEXT("Car"), VelocitySchema);
	//SchemaMapping.Add(TEXT("Collision"), CollisionSchema);
	SchemaMapping.Add(TEXT("Terrain"), TerrainSchema);

	OutObservationSchemaElement = ULearningAgentsObservations::SpecifyStructObservation(InObservationSchema, SchemaMapping);

	CollisionFOVOffsets.SetNum(CollisionRaycastCount);
	for (int i = 0; i < CollisionRaycastCount; ++i)
	{
		// Pre-calculate the relative yaw offset in degrees
		CollisionFOVOffsets[i] = CollisionFOVStartAngle + i * CollisionFOVStep;
	}
	
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
	
	//TArray<FLearningAgentsObservationObjectElement> TrackObservations;
	TMap<FName, FLearningAgentsObservationObjectElement> ObjectMapping;
	FVector Location = ObservationActor->GetActorLocation();
	FTransform Transform = ObservationActor->GetActorTransform();
	//float DistanceAlongSpline = TrackSpline->GetDistanceAlongSplineAtLocation(Location, ESplineCoordinateSpace::World);
	bool bIsPlayerControlled = ObservationActor->IsPlayerControlled();
	//for(float TrackDistance : TrackDistances)
	//{
	//	float SampleDistance = DistanceAlongSpline + TrackDistance;
	//	FVector LocationAtDistanceAlongSpline = TrackSpline->GetLocationAtDistanceAlongSpline(SampleDistance, ESplineCoordinateSpace::World);
	//	
	//	FLearningAgentsObservationObjectElement LocationObject = ULearningAgentsObservations::MakeLocationAlongSplineObservation(InObservationObject, TrackSpline, SampleDistance, Transform, TEXT("LocationAlongSplineObservation"), bIsPlayerControlled, this, AgentId, LocationAtDistanceAlongSpline, FLinearColor::Blue);
	//	FLearningAgentsObservationObjectElement DirectionObject = ULearningAgentsObservations::MakeDirectionAlongSplineObservation(InObservationObject, TrackSpline, SampleDistance, Transform, TEXT("DirectionAlongSplineObservation"), bIsPlayerControlled, this, AgentId, LocationAtDistanceAlongSpline, 100.0f, FLinearColor::Yellow);

	//	ObjectMapping.Add(TEXT("Location"), LocationObject);
	//	ObjectMapping.Add(TEXT("Direction"), DirectionObject);

	//	FLearningAgentsObservationObjectElement TrackObject = ULearningAgentsObservations::MakeStructObservation(InObservationObject, ObjectMapping);
	//	TrackObservations.Add(TrackObject);
	//}
	//FLearningAgentsObservationObjectElement TrackObject = ULearningAgentsObservations::MakeStaticArrayObservation(InObservationObject, TrackObservations);

	FVector Velocity = ObservationActor->GetVelocity();
	FLearningAgentsObservationObjectElement VelocityObject = ULearningAgentsObservations::MakeVelocityObservation(InObservationObject, Velocity, Transform);

	//TArray<FLearningAgentsObservationObjectElement> CollisionObservations;
	//FHitResult CollisionHitResult;
	//for (int i = 0; i < CollisionRaycastCount; ++i)
	//{
	//	// Compute the new ray direction based on the rotated vector and use it to get RayEnd
	//	FRotator Rotation = ObservationActor->GetActorRotation();
	//	Rotation.Yaw += CollisionFOVOffsets[i];
	//	FVector RayDir = Rotation.Vector();
	//	RayDir.Z = FMath::Max(0.0f, RayDir.Z); // Prevent looking into the ground while breaking
	//	FVector RayEnd = Location + RayDir * CollisionRayLength;

	//	// Perform the line trace
	//	GetWorld()->LineTraceSingleByChannel(CollisionHitResult, Location, RayEnd, ECC_Visibility, CollisionParams);

	//	if (CollisionHitResult.bBlockingHit)
	//	{
	//		FLearningAgentsObservationObjectElement CollisionObject = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, CollisionHitResult.ImpactPoint, Transform, TEXT("LocationObservation"), bIsPlayerControlled, this, AgentId, CollisionHitResult.ImpactPoint, FLinearColor::Green);
	//		CollisionObservations.Add(CollisionObject);

	//		if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, CollisionHitResult.ImpactPoint, FColor::Red); }
	//	}
	//	else
	//	{
	//		// If no hit, use the ray end point as the observation
	//		FLearningAgentsObservationObjectElement CollisionObject = ULearningAgentsObservations::MakeLocationObservation(InObservationObject, RayEnd, Transform, TEXT("LocationObservation"), bIsPlayerControlled, this, AgentId, RayEnd, FLinearColor::Green);
	//		CollisionObservations.Add(CollisionObject);

	//		if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, RayEnd, FColor::Red); }
	//	}
	//}
	//FLearningAgentsObservationObjectElement CollisionObject = ULearningAgentsObservations::MakeStaticArrayObservation(InObservationObject, CollisionObservations);

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
				//	//UMaterialInterface* SplineMaterial = LandscapeSplineActor->GetSplinesComponent()->GetSegments()[0]->SplineMeshes[0].Mesh->GetMaterial(0);
				//	FLearningAgentsObservationObjectElement TerrainObject = ULearningAgentsObservations::MakeFloatObservation(InObservationObject, 1.0f, TEXT("FloatObservation"), bIsPlayerControlled, this, AgentId, TerrainHitResult.ImpactPoint, FLinearColor::Black);
					//TerrainObservations.Add(TerrainObject);

					if (j == 0 && ObservationActor->bIsOffroad) { ObservationActor->bIsOffroad = false; }

					//if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, TerrainHitResult.ImpactPoint, FColor::Black); }
				//}
				//else
				//{
				//	FLearningAgentsObservationObjectElement TerrainObject = ULearningAgentsObservations::MakeFloatObservation(InObservationObject, 0.0f, TEXT("FloatObservation"), bIsPlayerControlled, this, AgentId, RayEnd, FLinearColor::Black);
				//	TerrainObservations.Add(TerrainObject);

				//	if (bIsPlayerControlled) { UKismetSystemLibrary::DrawDebugLine(GetWorld(), Location, TerrainHitResult.ImpactPoint, FColor::Black); }
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

	//ObjectMapping.Empty();
	//ObjectMapping.Add(TEXT("Track"), TrackObject);
	ObjectMapping.Add(TEXT("Car"), VelocityObject);
	//ObjectMapping.Add(TEXT("Collision"), CollisionObject);
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