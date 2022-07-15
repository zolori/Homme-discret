// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameModeBase.h"
#include "SurvivalGameState.h"
#include "MainMenuGameState.h"
#include "StealthGameMode.generated.h"

UENUM(BlueprintType)
enum GameStates
{
	MAINMENU UMETA(DisplayName = "Main Menu"),
	SURVIVAL UMETA(DisplayName = "Survival"),
	SCORING UMETA(DisplayName = "Score")
};

/**
 *
 */
class AFoe;
class AFoeSpawner;
UCLASS()
class HOMMEDISCRET_API AStealthGameMode : public AGameModeBase
{
	GENERATED_BODY()

	//Call when the game start
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
		ASurvivalGameState* SurvivalGameState;

	//FTimerHandle TimerHandle;
	TArray<FTimerHandle> Timers;

	void SetGameOver(bool value);
	bool GetGameOver();
	void SetWon(bool value);
	bool GetWon();
	int getCurrentFoodsInChest();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> PlayerHUDClass;

	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> PauseHUDClass;
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> VictoryHUDclass;
		UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> DefeatHUDClass;
	UPROPERTY()
		UUserWidget* PlayerWidget;
	UPROPERTY()
		UUserWidget* PauseWidget;
	UPROPERTY()
		UUserWidget* EndingWidget;


	//The number of food that are needed for the player to be declared the winner. Defaults = 5
	UPROPERTY(EditAnywhere, Category = "Victory Condition")
		int32 NumberOfFoodInChestForVictory = 5;

	UPROPERTY(EditAnywhere)
		int FoodInChestToWin = 5;

	UPROPERTY(EditAnywhere)
		int MaxFoodsInRoom = 5;

	class UHDGameInstance* GameInstance = Cast<UHDGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));

public:
	AStealthGameMode();

	virtual ~AStealthGameMode() = default;

	UFUNCTION(Category = "Victory Condition")
		virtual void PlayerWon();

	UFUNCTION(Category = "Defeat Condition")
		void LostGame();

	UFUNCTION(Category = "Food")
		void AddFoodInChest(int FoodValue);

	UFUNCTION(Category = "Food")
		void AddFoodInRoom();

	UFUNCTION(Category = "Food")
		void RemoveFoodInRoom();

	UFUNCTION(Category = "Foe")
		FTimerHandle GetTimerHandle(AFoe* Foe);

	UFUNCTION(Category = "FoeController")
		TArray<AAIC_Foe*> GetFoeControllers();

	UFUNCTION(Category = "FoeController")
		void AddFoeController(AAIC_Foe* NewFoeController);

	UFUNCTION(Category = "FoeController")
		void RemoveFoeController(AAIC_Foe* OldFoeController);

	UFUNCTION(Category = "FoeController")
		AAIC_Foe* FindFreeFoeController();

	UFUNCTION(Category = "Foe")
		int SpawnFoe(int CurrentIndex);

	UFUNCTION(Category="Foe")
	int SpawnAllFoes(int NumberOfFoes);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Foe")
		UClass*  FoeToSpawn;

	void SetAIWaiting(AFoe* Foe);

	UFUNCTION(Category = "Foe")
		void AddFoeInRoom();

	UFUNCTION(Category = "Foe")
		void RemoveFoeInRoom(AFoe* Foe);

	UFUNCTION(Category = "Foe")
		void SetFoeCarryFood(bool NewNextFood);

	UFUNCTION(Category = "Foe")
		bool GetFoeCarryFood();

	UFUNCTION(Category = "Foe")
		void LaunchGameStateAI();

	UFUNCTION(Category = "Foe")
		void LaunchAI();

	UFUNCTION(Category = "Foe")
		void LaunchSpawnTimer(FTimerHandle Timer,float InRate, bool IsLooping);

	UFUNCTION(Category = "Foe")
		void LaunchTeleportTimer(FTimerHandle Timer,float InRate, bool IsLooping);

	AFoe* FoeToTeleport;

	UFUNCTION(Category = "Foe")
		void TeleportFoe();

	UFUNCTION(Category = "UI")
		virtual void ShowPauseMenu();

	UFUNCTION(Category = "UI")
		virtual void ShowNormalHUD();

	UFUNCTION(Category = "Navigation")
		void SetEnterLocation(FVector NewVector);

	UFUNCTION(Category = "Navigation")
		void SetExitLocation(FVector NewVector);

	UFUNCTION(Category = "Navigation")
		void SetOriginLocation(FVector NewVector);

	UFUNCTION(Category = "Navigation")
		FVector GetEnterLocation();

	UFUNCTION(Category = "Navigation")
		FVector GetRealEnterLocation(float SpaceBetween);

	UFUNCTION(Category = "Navigation")
		FVector GetExitLocation();

	UFUNCTION(Category = "Navigation")
		FVector GetOriginLocation();

	UFUNCTION(Category = "FoodSpot")
		TArray<AFoodSpot*> GetFoodSpotList();

	UFUNCTION(Category = "FoodSpot")
		AFoodSpot* GetOneRandomFoodSpot();

	UFUNCTION(Category = "UI")
		int getMaxFoodsInRoom();
};
