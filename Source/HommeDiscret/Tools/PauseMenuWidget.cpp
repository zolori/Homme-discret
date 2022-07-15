// Fill out your copyright notice in the Description page of Project Settings.


#include "PauseMenuWidget.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"
#include "Kismet/GameplayStatics.h"

void UPauseMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (ContinueGameButton)
	{
		ContinueGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnContinueGameClick);
	}
	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnRestartGameClick);
	}
	if (BackToMenuButton)
	{
		BackToMenuButton->OnClicked.AddDynamic(this, &UPauseMenuWidget::OnBackToMenuClick);
	}
}

void UPauseMenuWidget::OnContinueGameClick()
{
	AStealthGameMode* GM = Cast<AStealthGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	if (GM != nullptr)
	{
		GM->ShowNormalHUD();
	}
}

void UPauseMenuWidget::OnRestartGameClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGameLevel"));
}

void UPauseMenuWidget::OnBackToMenuClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
