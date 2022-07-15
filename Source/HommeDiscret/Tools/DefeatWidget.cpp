// Fill out your copyright notice in the Description page of Project Settings.


#include "DefeatWidget.h"
#include "Kismet/GameplayStatics.h"

void UDefeatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	if (RestartGameButton)
	{
		RestartGameButton->OnClicked.AddDynamic(this, &UDefeatWidget::OnRestartGameClick);
	}
	if (BackToMenuButton)
	{
		BackToMenuButton->OnClicked.AddDynamic(this, &UDefeatWidget::OnBackToMenuClick);
	}
}

void UDefeatWidget::OnRestartGameClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGameLevel"));
}

void UDefeatWidget::OnBackToMenuClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("MainMenu"));
}
