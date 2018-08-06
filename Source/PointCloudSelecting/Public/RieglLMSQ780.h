// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "RieglLMSQ780.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POINTCLOUDSELECTING_API URieglLMSQ780 : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	URieglLMSQ780();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	void RecalcRotationMatrix();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// hyperparameters
	float Delta = 5.0f; // [m]
	FVector StartingPoint;
	float SpeedX = 90.0f; // [m/s]
	int SpeedAngular = 27; // [lines/s]
	int Frequency = 400000; // [Hz]
	int PhiMaxDegrees = 30; // [deg]
	float MinX = 0.0f;
	float MaxX = 100.0f;
	float MinY = 0.0f;
	float MaxY = 100.f;

private:

	// parameters
	float PhiMaxRadians;
	float Alpha;
	FMatrix RotationMatrix;
	float dPoint;
	FVector Down;
	FVector Direction;
	FVector CurrentPoint;

	// storage
	TArray<FVector> Points;
	TArray<float> Intensities;
};
