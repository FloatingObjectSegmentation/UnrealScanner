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
	Alpha = PhiMaxRadians * 2 * SpeedAngular / (float)Frequency;
	Down = FVector(0, 0, -1);
	Direction = FVector(0, 0, -1);
	RecalcRotationMatrix();


	// init plane velocity parameters
	StartingPoint = GetOwner()->GetActorLocation();
	CurrentPoint = StartingPoint;
	dPoint = SpeedX / (float)Frequency;

	MaxX += StartingPoint.X;
	MinX += StartingPoint.X;
	MinY += StartingPoint.Y;
	MaxY += StartingPoint.Y;
}

// Called every frame
void URieglLMSQ780::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	FVector RayCastStart, RayCastEnd;
	for (int i = 0; i < 1000; i++) {
		// stopping condition
		if (CurrentPoint.Y > MaxY)
			return;

		// update algorithm parameters
		CurrentPoint += FVector(dPoint, 0, 0);
		Direction = RotationMatrix.TransformVector(Direction);

		double thresh = PhiMaxRadians;
		double distance = acos(Dot3(Down, Direction));

		UE_LOG(LogTemp, Warning, TEXT("%f"), sqrt(pow(Direction.X, 2) + pow(Direction.Y, 2) + pow(Direction.Z, 2)));
		if (distance > thresh) {
			Alpha = -Alpha;
			RecalcRotationMatrix();
		}

		if (CurrentPoint.X > MaxX || CurrentPoint.X < MinX) {
			CurrentPoint += FVector(0, Delta, 0);
			dPoint = -dPoint;
		}

		// define raycast parameters
		RayCastStart = CurrentPoint;
		RayCastEnd = RayCastStart + Direction * pow(10, 6);
		ECollisionChannel Channel = ECollisionChannel::ECC_Visibility; // this won't actually have multi line trace!
	}
	DrawDebugLine(GetWorld(),
		RayCastStart,
		RayCastEnd,
		FColor(255, 0, 0),
		false,
		0.f,
		0.f,
		10.0f);

	// compute the hits and store them to array
	/*
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
	*/
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
