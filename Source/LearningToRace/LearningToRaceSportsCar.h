// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "LearningToRacePawn.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/SplineComponent.h"
#include "LearningAgentsManager.h"
#include "LearningToRaceGameMode.h"
#include "LearningToRaceSportsCar.generated.h"

/**
 *  Sports car wheeled vehicle implementation
 */
UCLASS(abstract)
class LEARNINGTORACE_API ALearningToRaceSportsCar : public ALearningToRacePawn
{
	GENERATED_BODY()
	
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadonly, Category = "Learning")
	bool bHasCrashed = false;

	ALearningToRaceSportsCar();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	void ResetToRandomPointOnSpline(USplineComponent* TrackSpline);

private:
	UFUNCTION()
	void OnHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);
};
