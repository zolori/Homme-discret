// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.h"
#include "Wall.h"
#include "Hideout.h"
#include "HommeDiscret/Level/Props/Crate.h"
#include "HommeDiscret/Level/Props/FoodSpot.h"
#include "HommeDiscret/IA/NavigationPoint.h"
#include "FoeSpawnerRoom.h"
#include "PlaygroundGenerator.generated.h"

UENUM(BlueprintType)
enum NearWhichWall
{
	BOTTOM UMETA(DisplayName = "Bottom Wall"),
	TOP UMETA(DisplayName = "Top Wall"),
	LEFT UMETA(DisplayName = "Left Wall"),
	RIGHT UMETA(DisplayName = "Right Wall"),
	TOP_LEFT_CORNER UMETA(DisplayName = "Top Left Corner"),
	TOP_RIGHT_CORNER UMETA(DisplayName = "Top Right Corner"),
	BOTTOM_LEFT_CORNER UMETA(DisplayName = "Bottom Left Corner"),
	BOTTOM_RIGHT_CORNER UMETA(DisplayName = "Bottom Right Corner"),
	NONE UMETA(DisplayName = "Not Near Wall")
};


UCLASS()
class HOMMEDISCRET_API APlaygroundGenerator : public AActor
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	APlaygroundGenerator();
	/*Reference the blueprint to generate the playground floor*/
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration")
		TSubclassOf<AFloorTile> FloorTile;
	/*Reference the blueprint to generate the playground wall*/
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration")
		TSubclassOf<AWall> Wall;
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration")
		TSubclassOf<AHideout> Hideout;
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration")
		TSubclassOf<AFoeSpawnerRoom> FoeSpawnerRoom;
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration")
		TSubclassOf<ANavigationPoint> NavigationPoint;
	/*Size of the tile to align the creation correctly*/
	UPROPERTY(VisibleAnywhere)
		float SizeOfTile = 200;
	UPROPERTY(EditAnywhere)
		UClass* SpawnActor;
	/*Set the number of vertical tile to create to fill the playground.*/
	UPROPERTY(VisibleAnywhere, Category = "PlaygroundGeneration", meta = (UIMin = "10", UIMax = "15"))
		int VerticalTileNumber = 15;
	/*Set the number of horizontal tile to create to fill the playground.*/
	UPROPERTY(VisibleAnywhere, Category = "PlaygroundGeneration", meta = (UIMin = "10", UIMax = "15"))
		int HorizontalTileNumber = 15;
	/*Height of the wall*/
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration", meta = (UIMin = "1", UIMax = "10"))
		int HeightTileNumber = 1;

	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration/Crates")
		TSubclassOf<ACrate> Crate;
	UPROPERTY(VisibleAnywhere, Category = "PlaygroundGeneration/Crates")
		int CrateNumber;
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration/Crates", meta = (UIMin = "20", UIMax = "50"))
		int ChanceToSpawnTwoCrates = 20;

	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration/FoodSpot")
		TSubclassOf<AFoodSpot> FoodSpot;
	UPROPERTY(EditAnywhere, Category = "PlaygroundGeneration/FoodSpot", meta = (UIMin = "10"))
		int NumberOfFoodSpot = 10;

	TArray<AFloorTile*> TileList;
	TArray<AWall*> WallList;
	TArray<ACrate*> CrateList;
	TArray<AFoodSpot*> FoodSpotList;
	AHideout* HideoutReferences;
	AFoeSpawnerRoom* FoeSpawnerRoomReferences;
	ANavigationPoint* NavigationPointReferences;

	int ActualCrateNumber = 0;

	AFloorTile* EnemySpawnEntranceFloorTile;

	int RandomTileHideout;
	int RandomTileFoeRoom;

	FActorSpawnParameters Params;
	bool FloorIsSet = false;
	FVector BaseLocation;
	FRotator BaseRotation;
	float Offset;
	float Radius = 40.f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	//Generate the playground with the size specified in the VerticalTileNumber and HorizontalTileNumber.
	void SpawnPlayground();
	void DespawnPlayground();
	
	void GenerateTopWall();
	void GenerateBottomWall();
	void GenerateRightWall();
	void GenerateLefttWall();
	
	

	void SpawnCratesActor(UClass* Actor, AFloorTile* Tile, FRotator Rotation, FActorSpawnParameters Parameter, int Height = 1);
	void SpawnFoodSpotActor(UClass* Actor, AFloorTile* Tile, FRotator Rotation, FVector Coordinate, FActorSpawnParameters Parameter, int CurrentFoodSpotSpawned);

	void SpawnFoodSpot();
	void SpawnCrates();
	void FixHoles();

	NearWhichWall IsTileAtWall(int TileNumber);
	FVector GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight);
};
