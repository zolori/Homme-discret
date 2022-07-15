// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaygroundGenerator.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "HommeDiscret/Tools/GameMode/SurvivalGameState.h"

// Sets default values
APlaygroundGenerator::APlaygroundGenerator()
{
	Params.Owner = this;
	Offset = 100.f;
}

// Called when the game starts or when spawned
void APlaygroundGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (!FloorIsSet)
	{
		SpawnPlayground();
	}
	//SpawnItem(Object);
	//CreateGrid();
	//PlacePointOnGrid();
}

void APlaygroundGenerator::SpawnPlayground()
{
	RandomTileHideout = FMath::RandRange(0, HorizontalTileNumber - 1);
	RandomTileFoeRoom = FMath::RandRange(0, HorizontalTileNumber - 1);
	CrateNumber = (VerticalTileNumber * HorizontalTileNumber) % 2 == 0 ? (VerticalTileNumber * HorizontalTileNumber) / 2.5 : (VerticalTileNumber * HorizontalTileNumber) / 2.5 + 1;
	if (!FloorIsSet)
	{
		BaseLocation.X = Offset;
		BaseLocation.Y = Offset;
		FloorIsSet = true;

		for (size_t i = 0; i < VerticalTileNumber; i++)
		{
			for (size_t j = 0; j < HorizontalTileNumber; j++)
			{
				AFloorTile* SpawnedTile = GetWorld()->SpawnActor<AFloorTile>(FloorTile, BaseLocation, BaseRotation, Params);
				BaseLocation.Y += SizeOfTile;
				if (SpawnedTile != nullptr)
				{
					SpawnedTile->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					TileList.Add(SpawnedTile);
				}

				if ((i == 0) && j == RandomTileHideout)
				{
					HideoutReferences = GetWorld()->SpawnActor<AHideout>(Hideout, FVector(BaseLocation.X - SizeOfTile, BaseLocation.Y - SizeOfTile, BaseLocation.Z), FRotator(0.f, 90.f, 0.f), Params);

					if (HideoutReferences != nullptr)
					{
						HideoutReferences->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
						TileList.Last()->bIsHoldingCrate = true;
						TileList.Last()->bIsHoldingFoodSpot = true;
					}		
				}
				if (i == (VerticalTileNumber - 1) && j == RandomTileFoeRoom)
				{
					FoeSpawnerRoomReferences = GetWorld()->SpawnActor<AFoeSpawnerRoom>(FoeSpawnerRoom, FVector(BaseLocation.X + SizeOfTile, BaseLocation.Y - SizeOfTile, BaseLocation.Z), FRotator(0.f, -90.f, 0.f), Params);
					if (FoeSpawnerRoomReferences != nullptr)
					{
						FoeSpawnerRoomReferences->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
						TileList.Last()->bIsHoldingCrate = true;
						TileList.Last()->bIsHoldingFoodSpot = true;
					}
				}
				if (i == 7  && j == 6)
				{
					NavigationPointReferences = GetWorld()->SpawnActor<ANavigationPoint>(NavigationPoint, BaseLocation, FRotator::ZeroRotator, Params);
					if (NavigationPointReferences != nullptr)
					{
						NavigationPointReferences->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					}
				}
			}
			BaseLocation.Y -= SizeOfTile * HorizontalTileNumber;
			BaseLocation.X += SizeOfTile;
		}
		GenerateTopWall();
		GenerateBottomWall();
		GenerateRightWall();
		GenerateLefttWall();

		while (ActualCrateNumber < CrateNumber)
		{
			SpawnCrates();
			ActualCrateNumber++;
		}
		FixHoles();
		SpawnFoodSpot();

		GetWorld()->GetFirstPlayerController()->GetPawn()->SetActorLocation(HideoutReferences->GetActorLocation());
		
		ASurvivalGameState* GS = Cast<ASurvivalGameState>(UGameplayStatics::GetGameState(GetWorld()));
		if (GS != nullptr)
		{
			GS->FoodSpotList = FoodSpotList;
			GS->OriginLocation = NavigationPointReferences->GetActorLocation();
			GS->ExitLocation = FoeSpawnerRoomReferences->ExitingPointActor->GetActorLocation();
			GS->EnterLocation = FoeSpawnerRoomReferences->StartingPointActor->GetActorLocation();
		}
	}
}

void APlaygroundGenerator::GenerateTopWall()
{
	FVector WallStartLocation;
	WallStartLocation.Set(VerticalTileNumber * SizeOfTile, Offset, Offset);
	FRotator WallRotation;
	WallRotation.Yaw = 90.f;
	WallRotation.Pitch = 0.f;
	WallRotation.Roll = 0.f;

	for (size_t i = 0; i < HeightTileNumber; i++)
	{
		for (size_t j = 0; j < HorizontalTileNumber; j++)
		{
			if (((i == 0) && (j != RandomTileFoeRoom)) || i != 0)
			{
				AWall* SpawnedWall = GetWorld()->SpawnActor<AWall>(Wall, WallStartLocation, WallRotation, Params);
				WallStartLocation.Y += SizeOfTile;

				if (SpawnedWall != nullptr)
				{
					SpawnedWall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					WallList.Add(SpawnedWall);
				}
			}
			else
			{
				WallStartLocation.Y += SizeOfTile;
			}
		}
		WallStartLocation.Y -= SizeOfTile * HorizontalTileNumber;
		WallStartLocation.Z += SizeOfTile;
	}
}

void APlaygroundGenerator::GenerateBottomWall()
{
	FVector WallStartLocation;
	WallStartLocation.Set(0.f, Offset, Offset);
	FRotator WallRotation;
	WallRotation.Yaw = 90.f;
	WallRotation.Pitch = 0.f;
	WallRotation.Roll = 0.f;

	for (size_t i = 0; i < HeightTileNumber; i++)
	{
		for (size_t j = 0; j < HorizontalTileNumber; j++)
		{
			if (((i == 0) && (j != RandomTileHideout)) || i != 0)
			{
				AWall* SpawnedWall = GetWorld()->SpawnActor<AWall>(Wall, WallStartLocation, WallRotation, Params);
				WallStartLocation.Y += SizeOfTile;

				if (SpawnedWall != nullptr)
				{
					SpawnedWall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
					WallList.Add(SpawnedWall);
				}
			}
			else
			{
				WallStartLocation.Y += SizeOfTile;
			}
		}
		WallStartLocation.Y -= SizeOfTile * HorizontalTileNumber;
		WallStartLocation.Z += SizeOfTile;
	}
}

void APlaygroundGenerator::GenerateRightWall()
{
	FVector WallStartLocation;
	WallStartLocation.Set(Offset, HorizontalTileNumber * SizeOfTile, Offset);

	for (size_t i = 0; i < HeightTileNumber; i++)
	{
		for (size_t j = 0; j < VerticalTileNumber; j++)
		{
			AWall* SpawnedWall = GetWorld()->SpawnActor<AWall>(Wall, WallStartLocation, FRotator::ZeroRotator, Params);
			WallStartLocation.X += SizeOfTile;

			if (SpawnedWall != nullptr)
			{
				SpawnedWall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				WallList.Add(SpawnedWall);
			}
		}
		WallStartLocation.X -= SizeOfTile * VerticalTileNumber;
		WallStartLocation.Z += SizeOfTile;
	}
}

void APlaygroundGenerator::GenerateLefttWall()
{
	FVector WallStartLocation;
	WallStartLocation.Set(Offset, 0.f, Offset);

	for (size_t i = 0; i < HeightTileNumber; i++)
	{
		for (size_t j = 0; j < VerticalTileNumber; j++)
		{
			AWall* SpawnedWall = GetWorld()->SpawnActor<AWall>(Wall, WallStartLocation, FRotator::ZeroRotator, Params);
			WallStartLocation.X += SizeOfTile;

			if (SpawnedWall != nullptr)
			{
				SpawnedWall->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
				WallList.Add(SpawnedWall);
			}
		}
		WallStartLocation.X -= SizeOfTile * VerticalTileNumber;
		WallStartLocation.Z += SizeOfTile;
	}
}

void APlaygroundGenerator::DespawnPlayground()
{
	if (TileList.Num() > 0)
	{
		for (AFloorTile* CurrentFloorTile : TileList)
		{
			CurrentFloorTile->Destroy();
		}
		TileList.Empty();
	}
	if (WallList.Num() > 0)
	{
		for (AWall* CurrentWallTile : WallList)
		{
			CurrentWallTile->Destroy();
		}
		WallList.Empty();
	}
	if (HideoutReferences != nullptr)
	{
		HideoutReferences->Destroy();
	}

	if (CrateList.Num() > 0)
	{
		for (ACrate* CurrentCrateTile : CrateList)
		{
			CurrentCrateTile->Destroy();
		}
		CrateList.Empty();
	}
	BaseLocation.X = 0.0f;
	BaseLocation.Y = 0.0f;
	FloorIsSet = false;
}

void APlaygroundGenerator::SpawnCrates()
{
	int RandomTile = FMath::RandRange(0, (VerticalTileNumber * HorizontalTileNumber) - 1);
	int ShouldSpawnTwoCrates = FMath::RandRange(0, 99);
	switch (IsTileAtWall(RandomTile))
	{
	case NearWhichWall::LEFT:
		if ((!TileList[RandomTile]->bIsHoldingCrate) && 
			(!TileList[RandomTile + 1]->bIsHoldingCrate) && 
			(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("LEFT | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("LEFT | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::RIGHT:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile - 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("RIGHT | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("RIGHT | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::BOTTOM:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile + 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::TOP:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile + 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::TOP_LEFT_CORNER:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile + 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP LEFT CORNER | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP LEFT CORNER | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::TOP_RIGHT_CORNER:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile - 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP RIGHT CORNER | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP RIGHT CORNER | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::BOTTOM_LEFT_CORNER:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile + 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM LEFT CORNER | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM LEFT CORNER | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::BOTTOM_RIGHT_CORNER:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			(!TileList[RandomTile - 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM RIGHT CORNER | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM RIGHT CORNER | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	case NearWhichWall::NONE:
		if ((!TileList[RandomTile]->bIsHoldingCrate) &&
			//(!TileList[RandomTile + 1]->bIsHoldingCrate) &&
			//(!TileList[RandomTile - 1]->bIsHoldingCrate) &&
			//(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) &&
			//(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingCrate) &&
			(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingCrate))
		{
			if (ShouldSpawnTwoCrates < ChanceToSpawnTwoCrates)
			{
				UE_LOG(LogTemp, Warning, TEXT("NONE | SPAWNCRATE 2 CRATES"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params, 2);
			}
			else
			{
				UE_LOG(LogTemp, Warning, TEXT("NONE | SPAWNCRATE"));
				SpawnCratesActor(Crate, TileList[RandomTile], FRotator::ZeroRotator, Params);
			}
		}
		break;
	}
}

void APlaygroundGenerator::SpawnFoodSpot()
{
	int CurrentFoodSpotSpawned = 0;
	while (CurrentFoodSpotSpawned < NumberOfFoodSpot)
	{
		int RandomTile = FMath::FRandRange(0, (VerticalTileNumber * HorizontalTileNumber) - 1);
		FVector UpperLeftTile(TileList[RandomTile]->GetCoordinate().X  + Offset - Radius, TileList[RandomTile]->GetCoordinate().Y - Offset + Radius, TileList[RandomTile]->GetCoordinate().Z);
		FVector LowerRightTile(TileList[RandomTile]->GetCoordinate().X - Offset + Radius, TileList[RandomTile]->GetCoordinate().Y + Offset - Radius, TileList[RandomTile]->GetCoordinate().Z);
		FVector RandomPointInTile = GetRandomPointInSquare(UpperLeftTile, LowerRightTile);
		switch (IsTileAtWall(RandomTile))
		{
		case NearWhichWall::LEFT:

			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!((TileList[RandomTile + 1]->bIsHoldingCrate) && 
					(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingCrate) && 
					(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingCrate))) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("LEFT | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::RIGHT:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!((TileList[RandomTile - 1]->bIsHoldingCrate) &&
					(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingCrate) &&
					(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingCrate))) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("RIGHT | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::BOTTOM:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!((TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) &&
					(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingCrate) &&
					(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingCrate))) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::TOP:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!((TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate) &&
					(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingCrate) &&
					(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingCrate))) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::TOP_LEFT_CORNER:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP LEFT CORNER | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::TOP_RIGHT_CORNER:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("TOP RIGHT CORNER | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::BOTTOM_LEFT_CORNER:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM LEFT CORNER | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::BOTTOM_RIGHT_CORNER:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("BOTTOM RIGHT CORNER | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		case NearWhichWall::NONE:
			if ((!TileList[RandomTile]->bIsHoldingCrate) &&
				(!((TileList[RandomTile + 1]->bIsHoldingCrate) && 
					(TileList[RandomTile - 1]->bIsHoldingCrate))) &&
				(!((TileList[RandomTile + VerticalTileNumber]->bIsHoldingCrate) && 
					(TileList[RandomTile - VerticalTileNumber]->bIsHoldingCrate))) &&
				(!TileList[RandomTile]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot) &&
				(!TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot))
			{
				UE_LOG(LogTemp, Warning, TEXT("NONE | SPAWNFOODSPOT"));
				SpawnFoodSpotActor(FoodSpot, TileList[RandomTile], FRotator::ZeroRotator, RandomPointInTile, Params, CurrentFoodSpotSpawned++);
				TileList[RandomTile + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile + VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber + 1]->bIsHoldingFoodSpot = true;
				TileList[RandomTile - VerticalTileNumber - 1]->bIsHoldingFoodSpot = true;
				DrawDebugCircle(GetWorld(), RandomPointInTile, Radius, 48, FColor::Red, true, -1.f, 0, 2.5f, FVector(0.0f, 1.0f, 0.0f), FVector(1.0f, 0.0f, 0.0f), true);
			}
			break;
		}
	}
}

void APlaygroundGenerator::FixHoles()
{
	for (AFloorTile* CurrentFloorTile : TileList)
	{
		if (!CurrentFloorTile->bIsHoldingCrate)
		{
			int CurrentTileNumber = TileList.Find(CurrentFloorTile);
			switch (IsTileAtWall(CurrentTileNumber))
			{
			case NearWhichWall::LEFT:
				if ((TileList[CurrentTileNumber + 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber + VerticalTileNumber]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("LEFT | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::RIGHT:
				if ((TileList[CurrentTileNumber - 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber + VerticalTileNumber]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("RIGHT | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::BOTTOM:
				if ((TileList[CurrentTileNumber + 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber + VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("BOTTOM | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::TOP:
				if ((TileList[CurrentTileNumber + 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("TOP| FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::TOP_LEFT_CORNER:
				if ((TileList[CurrentTileNumber + 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("TOP LEFT CORNER | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::TOP_RIGHT_CORNER:
				if ((TileList[CurrentTileNumber - 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("TOP RIGHT CORNER | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::BOTTOM_LEFT_CORNER:
				if ((TileList[CurrentTileNumber + 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber + VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("BOTTOM LEFT CORNER | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::BOTTOM_RIGHT_CORNER:
				if ((TileList[CurrentTileNumber - 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber + VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("BOTTOM RIGHT CORNER | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			case NearWhichWall::NONE:
				if ((TileList[CurrentTileNumber + 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - 1]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber + VerticalTileNumber]->bIsHoldingCrate) &&
					(TileList[CurrentTileNumber - VerticalTileNumber]->bIsHoldingCrate))
				{
					UE_LOG(LogTemp, Warning, TEXT("NONE | FIX HOLE"));
					SpawnCratesActor(Crate, TileList[CurrentTileNumber], FRotator::ZeroRotator, Params, 2);
				}
				break;
			}
		}
	}
}

void APlaygroundGenerator::SpawnCratesActor(UClass* Actor, AFloorTile* Tile, FRotator Rotation, FActorSpawnParameters Parameter, int Height)
{
	for (size_t i = 0; i < Height; i++)
	{
		ACrate* SpawnedActor = GetWorld()->SpawnActor<ACrate>(Crate, FVector(Tile->GetCoordinate().X, Tile->GetCoordinate().Y, Tile->GetCoordinate().Z + 160.f * i), Rotation, Parameter);
		Tile->bIsHoldingCrate = true;
		if (SpawnedActor != nullptr)
		{
			SpawnedActor->AttachToActor(Tile, FAttachmentTransformRules::KeepWorldTransform);
			CrateList.Add(SpawnedActor);
		}
		if (i >= 0)
		{
			Tile->bIsHoldingMultipleCrates = true;
		}
	}

}

void APlaygroundGenerator::SpawnFoodSpotActor(UClass* Actor, AFloorTile* Tile, FRotator Rotation, FVector Coordinate, FActorSpawnParameters Parameter, int CurrentFoodSpotSpawned)
{
	AFoodSpot* SpawnedActor = GetWorld()->SpawnActor<AFoodSpot>(FoodSpot, Coordinate, Rotation, Parameter);
	Tile->bIsHoldingFoodSpot = true;
	if (SpawnedActor != nullptr)
	{
		SpawnedActor->AttachToActor(Tile, FAttachmentTransformRules::KeepWorldTransform);
		FoodSpotList.Add(SpawnedActor);
		if (CurrentFoodSpotSpawned == 0)
		{
			UE_LOG(LogTemp, Warning, TEXT("blabla"));
			SpawnedActor->InstantiateFoodSpot();
		}
	}
	CurrentFoodSpotSpawned++;
}

NearWhichWall APlaygroundGenerator::IsTileAtWall(int TileNumber)
{
	bool bIsLeftWall = false;
	bool bIsRightWall = false;
	bool bIsTopWall = false;
	bool bIsBottomWall = false;

	NearWhichWall NearWall = NearWhichWall::NONE;

	switch (TileNumber % VerticalTileNumber)
	{
	case 0:
		NearWall = NearWhichWall::LEFT;
		bIsLeftWall = true;
		break;
	case 14:
		NearWall = NearWhichWall::RIGHT;
		bIsRightWall = true;
		break;
	}

	if ((TileNumber >= 0) && (TileNumber < VerticalTileNumber))
	{
		NearWall = NearWhichWall::BOTTOM;
		bIsBottomWall = true;
	}

	if ((TileNumber >= (VerticalTileNumber * HorizontalTileNumber) - VerticalTileNumber) && (TileNumber < (VerticalTileNumber* HorizontalTileNumber)))
	{
		NearWall = NearWhichWall::TOP;
		bIsTopWall = true;
	}

	if (bIsTopWall && bIsLeftWall)
	{
		NearWall = NearWhichWall::TOP_LEFT_CORNER;
	}
	else if (bIsTopWall && bIsRightWall)
	{
		NearWall = NearWhichWall::TOP_RIGHT_CORNER;
	}
	else if (bIsBottomWall && bIsLeftWall)
	{
		NearWall = NearWhichWall::BOTTOM_LEFT_CORNER;
	}
	else if (bIsBottomWall && bIsRightWall)
	{
		NearWall = NearWhichWall::BOTTOM_RIGHT_CORNER;
	}

	return NearWall;
}

FVector APlaygroundGenerator::GetRandomPointInSquare(const FVector& UpperLeft, const FVector& LowerRight)
{
	float RandomX = FMath::FRandRange(LowerRight.X, UpperLeft.X);
	float RandomY = FMath::FRandRange(UpperLeft.Y, LowerRight.Y);

	return FVector(RandomX, RandomY, 0.0f);
}
