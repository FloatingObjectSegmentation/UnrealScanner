// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PointCloud.h"
#include "PointCloudHelper.h"
#include "PointCloudActor.h"
#include "PointCloudFilteringComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POINTCLOUDSELECTING_API UPointCloudFilteringComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UPointCloudFilteringComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	UPointCloud* FilterPointCloud(UPointCloud* cloud, FVector* center, FVector* bounds);
	void SaveToDisc(UPointCloud* cloud);
		
};
