// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuGameMode.generated.h"

/**
 * 
 */
UCLASS()
class HOMMEDISCRET_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

	//Call when the game start
	virtual void BeginPlay() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
		TSubclassOf<UUserWidget> PlayerHUDClass;
	UPROPERTY()
		UUserWidget* CurrentWidget;

public:
	AMainMenuGameMode();

	virtual ~AMainMenuGameMode() = default;
};
