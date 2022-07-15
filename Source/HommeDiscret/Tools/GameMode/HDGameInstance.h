// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "HommeDiscret/Tools/HungerBar.h"
#include "HDGameInstance.generated.h"

/**
 * HungerBar = HungerBar
 */
UCLASS()
class HOMMEDISCRET_API UHDGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
protected:
	// Dynamic reference to the blueprint class
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UUserWidget> WBP_HungerBar;

	// Internal reference to the blueprint for gameplay logic
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "UI")
		class UHungerBar* HungerBar;

public:
	UHDGameInstance();
	FORCEINLINE class UHungerBar* GetHungerBar() const { return HungerBar; }

public:

	// Interface IInGameMenuInterface
	// Method callable from blueprite to create the UI and add it to the viewport
	UFUNCTION(BlueprintCallable)
		void LoadHungerBar();

};
