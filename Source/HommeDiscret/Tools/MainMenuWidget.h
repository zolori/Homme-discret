// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class HOMMEDISCRET_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:	

	UPROPERTY(meta = (BindWidget))
		UButton* StartButton;
	UPROPERTY(meta = (BindWidget))
		UButton* CloseButton;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* StartButtonText;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* CloseButtonText;
	UPROPERTY(meta = (BindWidget))
		class UImage* MainMenuImage;
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* MainMenuText;

	virtual void NativeConstruct() override;
public:
	UFUNCTION()
		void OnStartClick();
	UFUNCTION()
		void OnCloseClick();
};
