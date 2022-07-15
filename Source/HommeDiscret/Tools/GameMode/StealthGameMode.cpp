// Fill out your copyright notice in the Description page of Project Settings.

#include "StealthGameMode.h"
#include "HommeDiscret/IA/Foe.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "HDGameInstance.h"

AStealthGameMode::AStealthGameMode()
{
}

void AStealthGameMode::BeginPlay()
{
	Super::BeginPlay();
	SurvivalGameState = GetGameState<ASurvivalGameState>();
	GameInstance->LoadHungerBar();

	FInputModeGameOnly InputType;
	InputType.SetConsumeCaptureMouseDown(true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputType);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
	}

	if (PlayerHUDClass != nullptr)
	{
		PlayerWidget = GameInstance->GetHungerBar();
		if (PlayerWidget != nullptr)
		{
			PlayerWidget->AddToViewport();
		}
	}

	if (SpawnAllFoes(3) == 0)
	{
		LaunchGameStateAI();
	}
	//ShowNormalHUD();
	/*SpawnFoe(0);
	LaunchAI();*/
}

void AStealthGameMode::SetGameOver(bool value)
{
	SurvivalGameState->GameOver = true;
}

bool AStealthGameMode::GetGameOver()
{
	if(SurvivalGameState != nullptr)
		return SurvivalGameState->GameOver;
	return false;
}

void AStealthGameMode::SetWon(bool value)
{
	SurvivalGameState->Won = true;
}

bool AStealthGameMode::GetWon()
{
	if (SurvivalGameState != nullptr)
		return SurvivalGameState->Won;
	return false;
}

int AStealthGameMode::getCurrentFoodsInChest()
{
	return SurvivalGameState->FoodCountInChest;
}

void AStealthGameMode::PlayerWon()
{
	if (SurvivalGameState->FoodCountInChest == FoodInChestToWin)
	{
		SetWon(true);
		PlayerWidget->RemoveFromViewport();
		FInputModeGameAndUI InputType;
		InputType.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
		UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputType);
		APlayerController* PC = GetWorld()->GetFirstPlayerController();
		if (PC)
		{
			PC->bShowMouseCursor = true;
			PC->bEnableClickEvents = true;
			PC->bEnableMouseOverEvents = true;
		}
		if (VictoryHUDclass != nullptr)
		{
			EndingWidget = CreateWidget(GetWorld(), VictoryHUDclass);
			if (EndingWidget != nullptr)
			{
				EndingWidget->AddToViewport();
			}
		}
	}
}

void AStealthGameMode::LostGame()
{
	SetGameOver(true);
	PlayerWidget->RemoveFromViewport();
	FInputModeGameAndUI InputType;
	InputType.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputType);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
	if (DefeatHUDClass != nullptr)
	{
		EndingWidget = CreateWidget(GetWorld(), DefeatHUDClass);
		if (EndingWidget != nullptr)
		{
			EndingWidget->AddToViewport();
		}
	}
}

void AStealthGameMode::AddFoodInChest(int FoodValue)
{
	SurvivalGameState->FoodCountInChest += FoodValue;
	GameInstance->GetHungerBar()->setCurrentFood();
	GameInstance->GetHungerBar()->setProgressBar();
	RemoveFoodInRoom();
	PlayerWon();
}

void AStealthGameMode::AddFoodInRoom()
{
	SurvivalGameState->FoodCountInRoom++;

	if (SurvivalGameState->FoodCountInRoom >MaxFoodsInRoom)
	{
		SurvivalGameState->FoodCountInRoom = 5;
		SetFoeCarryFood(false);
	}
}

void AStealthGameMode::RemoveFoodInRoom()
{
	SurvivalGameState->FoodCountInRoom--;
	if (SurvivalGameState->FoodCountInRoom < MaxFoodsInRoom)
	{
		SetFoeCarryFood(true);
		if (SurvivalGameState->FoodCountInRoom <= 0)
		{
			SurvivalGameState->FoodCountInRoom = 0;
		}
	}
}

FTimerHandle AStealthGameMode::GetTimerHandle(AFoe* Foe)
{
	FTimerHandle CurrentTimer;
	AAIC_Foe* FoeController = Cast<AAIC_Foe>(Foe->GetController());
	int IndexTimer = GetFoeControllers().Find(FoeController);
	if (Timers.IsValidIndex(IndexTimer))
	{
		CurrentTimer = Timers[IndexTimer];
	}
	return CurrentTimer;
}

TArray<AAIC_Foe*> AStealthGameMode::GetFoeControllers()
{
	return SurvivalGameState->FoeControllerList;
}

void AStealthGameMode::AddFoeController(AAIC_Foe* NewFoeController)
{
	SurvivalGameState->FoeControllerList.Add(NewFoeController);
}

void AStealthGameMode::RemoveFoeController(AAIC_Foe* OldFoeController)
{
	SurvivalGameState->FoeControllerList.Remove(OldFoeController);
}

AAIC_Foe* AStealthGameMode::FindFreeFoeController()
{
	AAIC_Foe* FoeController = nullptr;
	TArray<AAIC_Foe*> theControllers = GetFoeControllers();
	bool End = false;
	int i = 0;
	int ControllerNumber = theControllers.Num();
	do
	{
		AAIC_Foe* CurrentController = theControllers[i];
		if (CurrentController->GetBehaviorTreeIsRunning() == false)
		{
			FoeController = CurrentController;
			End = true;
		}
		else
		{
			i++;
		}
	} while (i < ControllerNumber && End == false);
	return FoeController;
}

int AStealthGameMode::SpawnFoe(int CurrentIndex)
{
	int Result = 1;
	float NewSpaceBetween = CurrentIndex * 110.0f;
	FActorSpawnParameters SpawnParams;
	AActor* SpawnedFoeRef = GetWorld()->SpawnActor<AFoe>(FoeToSpawn, GetRealEnterLocation(NewSpaceBetween), FRotator::ZeroRotator, SpawnParams);
	if (SpawnedFoeRef != nullptr)
	{
		AFoe* NewFoe = Cast<AFoe>(SpawnedFoeRef);
		if (NewFoe != nullptr)
		{
			AAIC_Foe* NewFoeController = Cast<AAIC_Foe>(NewFoe->GetController());
			if (NewFoeController != nullptr)
			{
				AddFoeController(NewFoeController);
				FTimerHandle TimerHandle;
				Timers.Add(TimerHandle);
				NewFoe->SpaceBetween= NewSpaceBetween;
				Result = 0;
			}
		}
	}
	return Result;
}

int AStealthGameMode::SpawnAllFoes(int NumberOfFoes)
{
	int Result = 0;
	for (int i = 0;i < NumberOfFoes;i++)
	{
		Result+=SpawnFoe(i);
	}
	return Result;
}

void AStealthGameMode::SetAIWaiting(AFoe* Foe)
{
	RemoveFoeInRoom(Foe);
	if (Foe != nullptr)
	{
		FoeToTeleport = Foe;
		LaunchTeleportTimer(GetTimerHandle(Foe),1.0f, false);
	}
}

void AStealthGameMode::AddFoeInRoom()
{
	SurvivalGameState->FoeCountInRoom++;
}

void AStealthGameMode::RemoveFoeInRoom(AFoe* Foe)
{
	SurvivalGameState->FoeCountInRoom--;
	UE_LOG(LogTemp, Warning, TEXT("One Foe has gone"));
	if (SurvivalGameState->FoeCountInRoom <= 0)
	{
		LaunchAI();
	}
	else
	{
		FTimerHandle CurrentTimerHandle = GetTimerHandle(Foe);
		int RandomTimer = rand() % 6;
		if (RandomTimer>0)
		{
			LaunchSpawnTimer(CurrentTimerHandle, (float)RandomTimer, false);
		}
		else 
		{
			LaunchAI();
		}
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Timer %d"),RandomTimer));
	}
}

void AStealthGameMode::SetFoeCarryFood(bool NewNextFood)
{
	SurvivalGameState->FoeCarryFood = NewNextFood;
}

bool AStealthGameMode::GetFoeCarryFood()
{
	return SurvivalGameState->FoeCarryFood;
}

void AStealthGameMode::LaunchGameStateAI()
{
	LaunchAI();
	LaunchAI();
	LaunchSpawnTimer(Timers[2],60.0f, false);
}

void AStealthGameMode::LaunchAI()
{
	AAIC_Foe* FreeFoeController = FindFreeFoeController();
	if (FreeFoeController != nullptr)
	{
		if (GetFoeCarryFood())
		{
			AddFoodInRoom();
			//UE_LOG(LogTemp, Warning, TEXT("Foe with Food"));
			FreeFoeController->InstantiateFoodToFoe(GetFoeCarryFood());
		}
		FreeFoeController->StartAIBehavior();
		AddFoeInRoom();
	}
}

void AStealthGameMode::LaunchSpawnTimer(FTimerHandle Timer,float InRate, bool IsLooping)
{
	GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Timer Launched : %f"),InRate));
	GetWorldTimerManager().SetTimer(Timer, this, &AStealthGameMode::LaunchAI, InRate, IsLooping);
}

void AStealthGameMode::LaunchTeleportTimer(FTimerHandle Timer, float InRate, bool IsLooping)
{
	GetWorldTimerManager().SetTimer(Timer, this, &AStealthGameMode::TeleportFoe, InRate, IsLooping);
}

void AStealthGameMode::TeleportFoe()
{

	FoeToTeleport->SetActorLocation(GetRealEnterLocation(FoeToTeleport->SpaceBetween));
	FoeToTeleport = nullptr;
}

void AStealthGameMode::ShowPauseMenu()
{
	PlayerWidget->RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), true);
	FInputModeGameAndUI InputType;
	InputType.SetLockMouseToViewportBehavior(EMouseLockMode::LockAlways);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputType);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = true;
		PC->bEnableClickEvents = true;
		PC->bEnableMouseOverEvents = true;
	}
	//Load UI
	if (PauseHUDClass != nullptr)
	{
		PauseWidget = CreateWidget<UUserWidget>(GetWorld(), PauseHUDClass);
		if (PauseWidget != nullptr)
		{
			PauseWidget->AddToViewport();
		}
	}
}

void AStealthGameMode::ShowNormalHUD()
{
	PauseWidget->RemoveFromParent();
	UGameplayStatics::SetGamePaused(GetWorld(), false);
	FInputModeGameOnly InputType;
	InputType.SetConsumeCaptureMouseDown(true);
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(InputType);
	APlayerController* PC = GetWorld()->GetFirstPlayerController();
	if (PC)
	{
		PC->bShowMouseCursor = false;
		PC->bEnableClickEvents = false;
		PC->bEnableMouseOverEvents = false;
	}
	//Load UI
	if (PlayerHUDClass != nullptr)
	{
		PlayerWidget = CreateWidget<UUserWidget>(GetWorld(), PlayerHUDClass);
		if (PlayerWidget != nullptr)
		{
			PlayerWidget->AddToViewport();
		}
	}
}

void AStealthGameMode::SetEnterLocation(FVector NewVector)
{

	SurvivalGameState->EnterLocation=NewVector;
}

void AStealthGameMode::SetExitLocation(FVector NewVector)
{
	SurvivalGameState->ExitLocation = NewVector;
}

void AStealthGameMode::SetOriginLocation(FVector NewVector)
{
	SurvivalGameState->OriginLocation = NewVector;
}

FVector AStealthGameMode::GetEnterLocation()
{
	return SurvivalGameState->EnterLocation;
}

FVector AStealthGameMode::GetRealEnterLocation(float SpaceBetween)
{
	return FVector(GetEnterLocation().X + SpaceBetween, GetEnterLocation().Y, GetEnterLocation().Z);
}

FVector AStealthGameMode::GetExitLocation()
{
	return SurvivalGameState->ExitLocation;
}

FVector AStealthGameMode::GetOriginLocation()
{
	return SurvivalGameState->OriginLocation;
}

TArray<AFoodSpot*> AStealthGameMode::GetFoodSpotList()
{
	return SurvivalGameState->FoodSpotList;
}

AFoodSpot* AStealthGameMode::GetOneRandomFoodSpot()
{
	TArray<AFoodSpot*> theFoodSpots = GetFoodSpotList();
	int RandomIndex = rand() % theFoodSpots.Num();
	AActor* NewActor = theFoodSpots[RandomIndex];
	AFoodSpot* NewFoodSpot = Cast<AFoodSpot>(NewActor);
	if (NewFoodSpot == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("No FoodSpot Found"));
	}
	return NewFoodSpot;
}

int AStealthGameMode::getMaxFoodsInRoom()
{
	return MaxFoodsInRoom;

}
