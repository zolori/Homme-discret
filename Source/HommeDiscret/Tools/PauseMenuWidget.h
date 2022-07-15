// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PauseMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOMMEDISCRET_API UPauseMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(meta = (BindWidget))
		UButton* ContinueGameButton;
	UPROPERTY(meta = (BindWidget))
		UButton* RestartGameButton;
	UPROPERTY(meta = (BindWidget))
		UButton* BackToMenuButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ContinueGameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* RestartGameText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* BackToMenuText;
	APlayerController* PC;

public:
	virtual void NativeConstruct() override;

	UFUNCTION()
		void OnContinueGameClick();
	UFUNCTION()
		void OnRestartGameClick();
	UFUNCTION()
		void OnBackToMenuClick();
};
