// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include "Rpc.h"

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointCloud.h"
#include "Runtime/Core/Public/Misc/FileHelper.h"
#include "Runtime/Core/Public/Misc/Guid.h"
#include "Runtime/Core/Public/Containers/UnrealString.h"
#include "Runtime/Core/Public/Misc/StringFormatArg.h"
#include "PointCloudHelper.h"
#include "PointCloudActor.h"
#include "Engine/World.h"
#include "PointCloudRenderingComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POINTCLOUDSELECTING_API UPointCloudRenderingComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	APointCloudActor * PointCloudHostActor;
	TArray<FPointCloudPoint> LoadedPoints;
	UPointCloud* PointCloud;
	float MaxX;
	float MinY;
	float MinZ;

	FString PointCloudFile = TEXT("C:\\Users\\km\\Desktop\\playground\\unreal\\unreal_workspaces\\PointCloudia\\simon.txt");
	FString PointCloudClassFile = TEXT("C:\\Users\\km\\Desktop\\playground\\unreal\\unreal_workspaces\\PointCloudia\\simonclass.txt");
	

public:	
	UPointCloudRenderingComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public: // API

	UFUNCTION(BlueprintCallable)
	FString ProcessSelectedPoints(FVector& CenterInWorldSpace, FVector& BoundingBox);

protected: // auxiliary
	void SpaceTransformPCToLocal(TArray<FPointCloudPoint> &LoadedPoints);
	UPointCloud * PrepareRenderingSettings(TArray<FPointCloudPoint> &Points, FString pointCloudName, FString settingsName);
	void SpawnPointCloudHostActor(FTransform const &SpawningTransform);
	void GetPointCloudPoints(TArray<FPointCloudPoint> &LoadedPoints);
	TArray<FPointCloudPoint> LoadPointCloudFromFileTXT(FString filename, FVector2D RgbRange = FVector2D(0.0f, 256.0f * 256.0f - 1.0f));
	void FindExtremes(TArray<FPointCloudPoint> & LoadedPoints);
	void MarkSubsetWithinLoadedPoints(TArray<int32> &QueryResultIndices);
	TArray<FPointCloudPoint> GetPointSubset(TArray<int32> &QueryResultIndices);
	void FindSelectionIndices(FVector & CenterInWorldSpace, FVector & BoundingBox, TArray<int32> &QueryResultIndices);
	void RerenderPointCloud();
	FString SelectedPointsToPointCloudTxtFormatString(TArray<FPointCloudPoint> PointsToSave);
};
