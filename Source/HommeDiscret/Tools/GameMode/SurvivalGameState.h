// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "HommeDiscret/IA/AIC_Foe.h"
#include "HommeDiscret/Level/Props/FoodSpot.h"
#include "SurvivalGameState.generated.h"

/**
 *
 */
UCLASS()
class HOMMEDISCRET_API ASurvivalGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ASurvivalGameState();
	virtual ~ASurvivalGameState() = default;
	//Check the number of food that are present in the chest

	UPROPERTY(VisibleAnywhere)
		int FoodCountInChest = 0;

	UPROPERTY(VisibleAnywhere)
		int FoodCountInRoom = 0;

	UPROPERTY(VisibleAnywhere)
		int FoeCountInRoom = 0;

	UPROPERTY(VisibleAnywhere)
		bool FoeCarryFood = true;

	UPROPERTY()
		TArray<AFoodSpot*> FoodSpotList;

	UPROPERTY(VisibleAnywhere, Category = "NavigationPoint")
		FVector EnterLocation;

	UPROPERTY(VisibleAnywhere, Category = "NavigationPoint")
		FVector ExitLocation;

	UPROPERTY(VisibleAnywhere, Category = "NavigationPoint")
		FVector OriginLocation;

	UPROPERTY(VisibleAnywhere)
		TArray<AAIC_Foe*> FoeControllerList;

	UPROPERTY(VisibleAnywhere)
		bool GameOver;

	UPROPERTY(VisibleAnywhere)
		bool Won;
};
