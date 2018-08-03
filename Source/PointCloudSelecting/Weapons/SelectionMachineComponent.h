// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/StaticMeshActor.h"
#include "Engine/World.h"
#include "Engine/WorldComposition.h"
#include "Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Materials/Material.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Materials/MaterialInstanceDynamic.h"
#include "../PointCloudRenderingComponent.h"
#include "SelectionMachineComponent.generated.h"

UENUM(BlueprintType)		//"BlueprintType" is essential to include
enum class ETransformEnum : uint8
{
	Translation 	UMETA(DisplayName = "Translation"),
	Rotation    	UMETA(DisplayName = "Rotation"),
	Scaling  	    UMETA(DisplayName = "Scaling")
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class POINTCLOUDSELECTING_API USelectionMachineComponent : public UActorComponent
{
	GENERATED_BODY()

// UNREAL DEFAULT
public:	
	// Sets default values for this component's properties
	USelectionMachineComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;



// VARIABLES
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETransformEnum TransformMode;

private:
	float Speed = 1.0f;
	float SpawnDistance = 200.0f;
	float Health = 60.0f;
	bool IsDying = false;

	AActor* BoundingBox;
	UStaticMeshComponent* BBStaticMeshComponent;
	UMaterialInstanceDynamic* BBMaterial;
	FString BBMaterialPath;
	UPointCloudRenderingComponent* PointCloudComponent;

	UStaticMesh *DesiredStaticMesh; // bounding box template - it will be spawned using this object

// METHODS
public:
	UFUNCTION(BlueprintCallable)
	void SetMode(ETransformEnum tmode);

	UFUNCTION(BlueprintCallable)
	void StartSelection();

	UFUNCTION(BlueprintCallable)
	FString FinishSelection();

	UFUNCTION(BlueprintCallable)
	void TransformX(int32 way);

	UFUNCTION(BlueprintCallable)
	void TransformY(int32 way);

	UFUNCTION(BlueprintCallable)
	void TransformZ(int32 way);

private:
	void SetBoundingBoxAttributes();
	UMaterialInstanceDynamic* GetMaterialInstance();
	AActor * SpawnBoundingBox();
	void GetBoundingBoxSpawnTransform(FTransform &SpawningTransform);
	void CommitTransformation(FVector trans);
	void InitializeBoundingBoxTemplate();
	void FindTemplateMesh(TArray<AActor *> &FoundActors);
	void ExtractStaticMeshFromActor(TArray<AActor *> & FoundActors, const int32 &i);
	UPointCloudRenderingComponent* GetPointCloudRenderingComponent();
	void DestroyAndReinitialize();
	void KillALittleMore();
};
