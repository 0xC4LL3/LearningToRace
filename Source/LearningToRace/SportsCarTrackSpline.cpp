// Fill out your copyright notice in the Description page of Project Settings.


#include "SportsCarTrackSpline.h"

// Sets default values
ASportsCarTrackSpline::ASportsCarTrackSpline()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	TrackSpline = CreateDefaultSubobject<USplineComponent>(TEXT("TrackSpline"));
}

// Called when the game starts or when spawned
void ASportsCarTrackSpline::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASportsCarTrackSpline::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

