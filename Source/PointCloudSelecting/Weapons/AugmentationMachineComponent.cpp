// Fill out your copyright notice in the Description page of Project Settings.

#include "AugmentationMachineComponent.h"


// Sets default values for this component's properties
UAugmentationMachineComponent::UAugmentationMachineComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


#pragma region [unreal events]

// Called when the game starts
void UAugmentationMachineComponent::BeginPlay()
{
	Super::BeginPlay();

	LoadAirplaneStaticMesh();
	Airplane = SpawnBoundingBox();
	SetBoundingBoxAttributes();
}


// Called every frame
void UAugmentationMachineComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}
#pragma endregion

#pragma region [API]
void UAugmentationMachineComponent::TransformX(int32 way)
{
	if (way == 0) return;
	if (Airplane == nullptr) return;
	Airplane->SetActorLocation(Airplane->GetActorLocation() + FVector(way * 50.0f, 0, 0));
}
void UAugmentationMachineComponent::TransformY(int32 way)
{
	if (way == 0) return;
	if (Airplane == nullptr) return;
	Airplane->SetActorLocation(Airplane->GetActorLocation() + FVector(0, way * 50.0f, 0));
}
void UAugmentationMachineComponent::TransformYaw(int32 way)
{
	if (way == 0) return;
	if (Airplane == nullptr) return;
	Airplane->SetActorRotation(Airplane->GetActorRotation() + FRotator(0, 0.3 * way, 0));
}

void UAugmentationMachineComponent::StartScanning()
{
	URieglLMSQ780* comp = NewObject<URieglLMSQ780>(Airplane);
	comp->RegisterComponent();
	Airplane->AddOwnedComponent(comp);
}
#pragma endregion

#pragma region [auxiliary]

#pragma region [airplane mesh loading]
void UAugmentationMachineComponent::LoadAirplaneStaticMesh()
{
	TSubclassOf<AActor> ClassToFind = AActor::StaticClass();
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);

	FindTemplateMesh(FoundActors);
}

void UAugmentationMachineComponent::FindTemplateMesh(TArray<AActor *> &FoundActors)
{
	// This is a temporary and bad solution - it absolutely requires a simple cube to be
	// in the scene.

	for (int32 i = 0; i < FoundActors.Num(); i++) {
		FString str = FoundActors[i]->GetActorLabel();
		if (str.Contains(TEXT("Pointer"))) {
			ExtractStaticMeshFromActor(FoundActors, i);
			break;
		}
	}
}

void UAugmentationMachineComponent::ExtractStaticMeshFromActor(TArray<AActor *> & FoundActors, const int32 &i)
{
	TArray<UStaticMeshComponent*> Components;
	FoundActors[i]->GetComponents<UStaticMeshComponent>(Components);
	if (Components.Num() > 0) {
		AirplaneStaticMesh = Components[0]->GetStaticMesh();
		//UE_LOG(LogTemp, Warning, TEXT("MESH EXTRACTED!"));
	}
}
#pragma endregion

#pragma region [spawning airplane]
AActor* UAugmentationMachineComponent::SpawnBoundingBox()
{
	FTransform SpawningTransform;
	GetBoundingBoxSpawnTransform(OUT SpawningTransform);

	UClass* param = AStaticMeshActor::StaticClass();
	AActor* spawned = GetWorld()->SpawnActor(param, &SpawningTransform, FActorSpawnParameters());
	return spawned;
}

void UAugmentationMachineComponent::GetBoundingBoxSpawnTransform(FTransform &SpawningTransform)
{
	FVector SpawningLocation;
	FVector PlayerViewPointLocation;
	FRotator Rotation;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PlayerViewPointLocation, OUT Rotation); // OUT does nothing, but it allows us to mark out params!
	FVector RotAsVector = Rotation.Vector();
	SpawningLocation = GetOwner()->GetActorLocation() + SpawnDistance * RotAsVector;
	SpawningTransform.SetLocation(SpawningLocation);
}

void UAugmentationMachineComponent::SetBoundingBoxAttributes()
{
	TArray<UStaticMeshComponent*> Components;
	Airplane->GetComponents<UStaticMeshComponent>(Components);
	for (int32 i = 0; i < Components.Num(); i++) {
		UStaticMeshComponent* StaticMeshComponent = Components[i];
		StaticMeshComponent->SetMobility(EComponentMobility::Movable);
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		StaticMeshComponent->SetStaticMesh(AirplaneStaticMesh);
		StaticMeshComponent->SetWorldRotation(FQuat::MakeFromEuler(FVector(90, 0, 0)));


		// set the target material
		//BBMaterial = GetMaterialInstance();
		//StaticMeshComponent->SetMaterial(0, BBMaterial);
	}
}
#pragma endregion
#pragma endregion
