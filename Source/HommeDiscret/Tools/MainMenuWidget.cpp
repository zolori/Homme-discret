// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuWidget.h"
#include "Kismet/GameplayStatics.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"


void UMainMenuWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (StartButton)
	{
		StartButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnStartClick);
	}
	if (CloseButton)
	{
		CloseButton->OnClicked.AddDynamic(this, &UMainMenuWidget::OnCloseClick);
	}
}

void UMainMenuWidget::OnStartClick()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("InGameLevel"));
}

void UMainMenuWidget::OnCloseClick()
{
	GetWorld()->GetFirstPlayerController()->ConsoleCommand("quit");
}
