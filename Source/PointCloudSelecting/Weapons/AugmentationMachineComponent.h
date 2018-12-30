// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/StaticMesh.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "../Public/RieglLMSQ780.h"
#include "AugmentationMachineComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POINTCLOUDSELECTING_API UAugmentationMachineComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UAugmentationMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UFUNCTION(BlueprintCallable)
		void TransformX(int32 way);

	UFUNCTION(BlueprintCallable)
		void TransformY(int32 way);

	UFUNCTION(BlueprintCallable)
		void TransformYaw(int32 way);

	UFUNCTION(BlueprintCallable)
		void StartScanning();

public:
	UStaticMesh* AirplaneStaticMesh;
	float SpawnDistance = 200.0f;
	AActor* Airplane;
	URieglLMSQ780* ScannerComponent;

private:
	void LoadAirplaneStaticMesh();
	void FindTemplateMesh(TArray<AActor *> &FoundActors);
	void ExtractStaticMeshFromActor(TArray<AActor *> & FoundActors, const int32 &i);
	AActor* SpawnBoundingBox();
	void GetBoundingBoxSpawnTransform(FTransform &SpawningTransform);
	void SetBoundingBoxAttributes();
		
	
};
