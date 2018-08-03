// Fill out your copyright notice in the Description page of Project Settings.

#include "Public/RieglLMSQ780.h"


// Sets default values for this component's properties
URieglLMSQ780::URieglLMSQ780()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void URieglLMSQ780::BeginPlay()
{
	Super::BeginPlay();

	// init laser rotation parameters
	PhiMaxRadians = PI * PhiMaxDegrees / 180.0;
	Alpha = PhiMaxRadians * 2 * SpeedAngular / (float) Frequency;
	Down = FVector(0, -1, 0);
	RecalcRotationMatrix();
	

	// init plane velocity parameters
	CurrentPoint = StartingPoint;
	dPoint = SpeedX / (float) Frequency;
}

// Called every frame
void URieglLMSQ780::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// stopping condition
	if (CurrentPoint.Z < MaxZ)
		return;

	// update algorithm parameters
	CurrentPoint += FVector(dPoint, 0, 0);
	Direction = RotationMatrix.TransformVector(Direction);

	if (Dot3(Down, Direction) > cos(PhiMaxRadians)) {
		Alpha = -Alpha;
		RecalcRotationMatrix();
	}

	if (CurrentPoint.X > MaxX || CurrentPoint.X < MinX) {
		CurrentPoint += FVector(0, 0, Delta);
		dPoint = -dPoint;
	}

	// define raycast parameters
	FVector RayCastStart = CurrentPoint;
	FVector RayCastEnd = RayCastStart + Direction * pow(10, 6);
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility; // this won't actually have multi line trace!

	// compute the hits and store them to array
	TArray<FHitResult> Hits;
	GetWorld()->LineTraceMultiByChannel(Hits, RayCastStart, RayCastEnd, Channel);
	for (int32 i = 0; i < Hits.Num(); i++) {

		FVector SurfaceNormal = Hits[i].ImpactNormal;
		FVector HitDirection = -Direction;
		SurfaceNormal.Normalize();
		HitDirection.Normalize();

		float intensity = Dot3(SurfaceNormal, HitDirection);
		FVector location = Hits[i].Location;
		
		Points.Add(location);
		Intensities.Add(intensity);
	}
}

#pragma region [auxiliary]
void URieglLMSQ780::RecalcRotationMatrix()
{
	RotationMatrix = FMatrix(FPlane(1, 0, 0, 0),
		FPlane(0, cos(Alpha), -sin(Alpha), 0),
		FPlane(0, sin(Alpha), cos(Alpha), 0),
		FPlane(0, 0, 0, 1));
}
#pragma endregion
