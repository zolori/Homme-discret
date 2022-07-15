// Fill out your copyright notice in the Description page of Project Settings.


#include "VictoryWidget.h"
#include "Kismet/GameplayStatics.h"

void UVictoryWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UVictoryWidget::OnRestartGameClick);
	}
	if (BackToMenuButton)
	{
		BackToMenuButton->OnClicked.AddDynamic(this, &UVictoryWidget::OnBackToMenuClick);
	}
}

void UVictoryWidget::OnRestartGameClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGameLevel"));
}

void UVictoryWidget::OnBackToMenuClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}


