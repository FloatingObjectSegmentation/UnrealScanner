// Fill out your copyright notice in the Description page of Project Settings.

#include "PointCloudFilteringComponent.h"


// Sets default values for this component's properties
UPointCloudFilteringComponent::UPointCloudFilteringComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

#pragma region unreal events
void UPointCloudFilteringComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UPointCloudFilteringComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
#pragma endregion

#pragma region API
UPointCloud* UPointCloudFilteringComponent::FilterPointCloud(UPointCloud* cloud, FVector* center, FVector* bounds)
{
	return nullptr;
}

void UPointCloudFilteringComponent::SaveToDisc(UPointCloud* cloud)
{
	FString a = TEXT("a");
}
#pragma endregion

