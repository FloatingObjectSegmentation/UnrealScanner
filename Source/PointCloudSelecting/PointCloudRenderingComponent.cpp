// Fill out your copyright notice in the Description page of Project Settings.

#include "PointCloudRenderingComponent.h"


// Sets default values for this component's properties
UPointCloudRenderingComponent::UPointCloudRenderingComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


#pragma region unreal events
void UPointCloudRenderingComponent::BeginPlay()
{
	Super::BeginPlay();

	GetPointCloudPoints(LoadedPoints);
	FindExtremes(LoadedPoints); // Needed to be able to compute transformations between PC, Local and World space
	SpaceTransformPCToLocal(LoadedPoints);
	PointCloud = PrepareRenderingSettings(LoadedPoints, TEXT("PointCloud1"), TEXT("Settings1")); // use a predefined configuration (for now)
	SpawnPointCloudHostActor(FTransform(FVector(0.0f)));
	PointCloudHostActor->SetPointCloud(PointCloud);

	UE_LOG(LogTemp, Warning, TEXT("Point loaded %s"), *LoadedPoints[0].Color.ToString());
}

void UPointCloudRenderingComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}
#pragma endregion

#pragma region API
FString UPointCloudRenderingComponent::ProcessSelectedPoints(FVector& CenterInWorldSpace, FVector& BoundingBox)
{
	UE_LOG(LogTemp, Warning, TEXT("Inside ProcessSelectedPoints"));
	TArray<int32> QueryResultIndices;
	FindSelectionIndices(CenterInWorldSpace, BoundingBox, QueryResultIndices);
	TArray<FPointCloudPoint> SelectedPoints = GetPointSubset(QueryResultIndices);
	FString PointCloudTxt = SelectedPointsToPointCloudTxtFormatString(SelectedPoints);
	MarkSubsetWithinLoadedPoints(QueryResultIndices);
	RerenderPointCloud();
	return PointCloudTxt;
}
#pragma endregion

#pragma region auxiliary
UPointCloud* UPointCloudRenderingComponent::PrepareRenderingSettings(TArray<FPointCloudPoint> &Points, FString pointCloudName, FString settingsName)
{
	UPointCloud* PointCloud = NewObject<UPointCloud>(this->StaticClass(), *pointCloudName);
	UPointCloudSettings* PointCloudSettings = NewObject<UPointCloudSettings>(this->StaticClass(), *settingsName);
	PointCloudSettings->RenderMethod = EPointCloudRenderMethod::Sprite_Unlit_RGB;
	PointCloudSettings->SpriteSize = FVector2D(0.4f, 0.4f);
	PointCloudSettings->Scale = FVector(1.0f);
	PointCloudSettings->Brightness = 8.f;
	PointCloudSettings->Saturation = 4.5f;
	PointCloudSettings->SectionSize = FVector(100.f);
	PointCloud->SetSettings(PointCloudSettings);
	PointCloud->SetPointCloudData(Points, true);
	return PointCloud;
}

void UPointCloudRenderingComponent::SpawnPointCloudHostActor(FTransform const &SpawningTransform)
{
	UClass* param = APointCloudActor::StaticClass();
	AActor* spawned = GetWorld()->SpawnActor(param, &SpawningTransform, FActorSpawnParameters());
	FVector f = spawned->GetActorLocation();
	PointCloudHostActor = dynamic_cast<APointCloudActor*>(spawned);
}

void UPointCloudRenderingComponent::GetPointCloudPoints(TArray<FPointCloudPoint> &LoadedPoints)
{
	LoadedPoints = LoadPointCloudFromFileTXT(PointCloudFile, FVector2D(0.0f, 256.0f * 256.0f - 1.0f));
	TArray<FPointCloudPoint> ClassPoints = LoadPointCloudFromFileTXT(PointCloudClassFile, FVector2D(0.0f, 255.0f));
	TArray<FPointCloudPoint> PointNoFloor;
	for (int i = 0; i < LoadedPoints.Num(); i++) {
		if (ClassPoints[i].Color.B != 2) {
			PointNoFloor.Add(LoadedPoints[i]);
		}
	}
	LoadedPoints = PointNoFloor;
}

TArray<FPointCloudPoint> UPointCloudRenderingComponent::LoadPointCloudFromFileTXT(FString filename, FVector2D RgbRange)
{
	EPointCloudColorMode Mode = EPointCloudColorMode::RGB;
	FPointCloudFileHeader Header = FPointCloudHelper::ReadFileHeader(filename);
	Header.RGBRange = RgbRange;

	TArray<int32> SelectedColumns;
	for (int32 i = 0; i < 6; i++) {
		SelectedColumns.Add(i);
	}
	Header.SelectedColumns = SelectedColumns;

	TArray<FPointCloudPoint> points;
	FPointCloudHelper::ImportAsText(filename, points, Mode, 0, 500000000, Header);
	return points;
}

void UPointCloudRenderingComponent::FindExtremes(TArray<FPointCloudPoint> & LoadedPoints)
{
	MaxX = INT32_MIN;
	MinY = INT32_MAX;
	MinZ = INT32_MAX;
	for (int32 i = 0; i < LoadedPoints.Num(); i++) {
		if (LoadedPoints[i].Location.X > MaxX) MaxX = LoadedPoints[i].Location.X; // reflect through X axis
		if (LoadedPoints[i].Location.Y < MinY) MinY = LoadedPoints[i].Location.Y;
		if (LoadedPoints[i].Location.Z < MinZ) MinZ = LoadedPoints[i].Location.Z;
	}
}

void UPointCloudRenderingComponent::SpaceTransformPCToLocal(TArray<FPointCloudPoint> &LoadedPoints) {

	// It appears that it needs to be reflected through the X axis to be correctly visualized.
	// Something may be wrong in the transformation, but as long as we are getting the correct
	// object out, it will suffice for this application.
	for (int32 i = 0; i < LoadedPoints.Num(); i++) {
		LoadedPoints[i].Location.X = MaxX - LoadedPoints[i].Location.X;
		LoadedPoints[i].Location.Y -= MinY;
		LoadedPoints[i].Location.Z -= MinZ;
	}

}

void UPointCloudRenderingComponent::MarkSubsetWithinLoadedPoints(TArray<int32> &QueryResultIndices)
{
	for (auto& i : QueryResultIndices) {
		LoadedPoints[i].Color.R = 255;
		LoadedPoints[i].Color.G = 0;
		LoadedPoints[i].Color.B = 0;
	}
}
TArray<FPointCloudPoint> UPointCloudRenderingComponent::GetPointSubset(TArray<int32> &QueryResultIndices)
{
	TArray<FPointCloudPoint> QueryResult;
	for (auto& i : QueryResultIndices) {
		// normalizing colors is not a side effect of this function. It just needs to be done so because
		// the point cloud plugin is made so.
		float normR = LoadedPoints[i].Color.R / 256.0f;
		float normG = LoadedPoints[i].Color.G / 256.0f;
		float normB = LoadedPoints[i].Color.B / 256.0f;
		QueryResult.Emplace(LoadedPoints[i].Location.X, LoadedPoints[i].Location.Y, LoadedPoints[i].Location.Z,
			normR, normG, normB);
	}
	return QueryResult;
}
void UPointCloudRenderingComponent::FindSelectionIndices(FVector & CenterInWorldSpace, FVector & BoundingBox, TArray<int32> &QueryResultIndices)
{
	for (int32 i = 0; i < LoadedPoints.Num(); i++) {

		if (CenterInWorldSpace.X + BoundingBox.X > LoadedPoints[i].Location.X &&
			CenterInWorldSpace.X - BoundingBox.X < LoadedPoints[i].Location.X &&
			CenterInWorldSpace.Y + BoundingBox.Y > LoadedPoints[i].Location.Y &&
			CenterInWorldSpace.Y - BoundingBox.Y < LoadedPoints[i].Location.Y &&
			CenterInWorldSpace.Z + BoundingBox.Z > LoadedPoints[i].Location.Z &&
			CenterInWorldSpace.Z - BoundingBox.Z < LoadedPoints[i].Location.Z)
		{
			QueryResultIndices.Add(i);
		}
	}
}
void UPointCloudRenderingComponent::RerenderPointCloud()
{
	TArray<FPointCloudPoint> NewPointCloud;
	for (int32 i = 0; i < LoadedPoints.Num(); i++) {
		float Rnorm = LoadedPoints[i].Color.R / 256.0f;
		float Gnorm = LoadedPoints[i].Color.G / 256.0f;
		float Bnorm = LoadedPoints[i].Color.B / 256.0f;
		NewPointCloud.Emplace(LoadedPoints[i].Location.X, LoadedPoints[i].Location.Y, LoadedPoints[i].Location.Z,
			Rnorm, Gnorm, Bnorm);
	}

	PointCloudHostActor->Destroy();
	PointCloudHostActor = nullptr;
	UPointCloud* tmpPointCloud = PrepareRenderingSettings(NewPointCloud, "PointCloud2", "Settings2");
	SpawnPointCloudHostActor(FTransform(FVector(0.0f)));
	PointCloudHostActor->SetPointCloud(tmpPointCloud);
}
FString UPointCloudRenderingComponent::SelectedPointsToPointCloudTxtFormatString(TArray<FPointCloudPoint> PointsToSave)
{
	// transform points to PC space
	for (int32 i = 0; i < PointsToSave.Num(); i++) {

		// locations
		PointsToSave[i].Location.X = MaxX - PointsToSave[i].Location.X;
		PointsToSave[i].Location.Y += MinY;
		PointsToSave[i].Location.Z += MinZ;

		// colors
		PointsToSave[i].Color.R = PointsToSave[i].Color.R * 255 * 255;
		PointsToSave[i].Color.G = PointsToSave[i].Color.G * 255 * 255;
		PointsToSave[i].Color.B = PointsToSave[i].Color.B * 255 * 255;
	}

	// save the points to disk
	FString total;
	for (auto& val : PointsToSave) 
	{
		FString line = FString::Printf(TEXT("%.2f %.2f %.2f %d %d %d"), val.Location.X, val.Location.Y, val.Location.Z, (int32)val.Color.R, (int32)val.Color.G, (int32)val.Color.B);
		total.Append(line);
		total.Append("\n");
	}

	/* Class doesn't save the file anymore, merely returns the string
	FGuid guid = FGuid::NewGuid();
	FString filename = SavingFolder;
	filename.Append("/");
	filename.Append(guid.ToString() + TEXT(".txt"));
	SavingFile = filename;
	*/
	return total;
}
#pragma endregion
