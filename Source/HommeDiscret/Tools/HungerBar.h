// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "HommeDiscret/Level/Props/Chest.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HommeDiscret/Level/Props/Food.h"
#include "GameMode/StealthGameMode.h"
#include "GameMode/SurvivalGameState.h"
#include "HungerBar.generated.h"

/**
 *
 */
UCLASS()
class HOMMEDISCRET_API UHungerBar : public UUserWidget
{
	GENERATED_BODY()

public:
	UFUNCTION()
		void OnFoodStocked();

protected:
	UPROPERTY(meta = (BindWidget))
		class UProgressBar* HungerBar;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CurrentHungerLabel;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MaxHungerLabel;
	UPROPERTY(VisibleAnywhere)
		AChest* Chest;

	AStealthGameMode* GameMode;
	float MaxFood;
	float CurrentFood;

	void NativeOnInitiliazed();

	virtual void NativeConstruct() override;

public:
	void setMaxFood();
	void setCurrentFood();
	void setProgressBar();
};
