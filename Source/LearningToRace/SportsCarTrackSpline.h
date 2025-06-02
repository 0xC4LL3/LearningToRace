// Copyright (c) 2025 0xC4LL3. Licensed under the MIT License. See LICENSE file for details.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "SportsCarTrackSpline.generated.h"

UCLASS()
class LEARNINGTORACE_API ASportsCarTrackSpline : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Learning")
	USplineComponent* TrackSpline;

	// Sets default values for this actor's properties
	ASportsCarTrackSpline();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
