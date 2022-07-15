// Fill out your copyright notice in the Description page of Project Settings.


#include "HungerBar.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include <string>


void UHungerBar::NativeConstruct()
{
	Super::NativeConstruct();

	Chest = Cast<AChest>(UGameplayStatics::GetActorOfClass(GetWorld(), AChest::StaticClass()));
	Chest->HungerBar_OnFoodStocked.AddUObject(this, &UHungerBar::OnFoodStocked); //see above in wiki

	GameMode = Cast<AStealthGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	setMaxFood();
	setCurrentFood();
	setProgressBar();
}

void UHungerBar::setMaxFood()
{
	MaxFood = GameMode->getMaxFoodsInRoom();
	FText intAsText = FText::AsNumber(MaxFood);
	MaxHungerLabel->SetText(intAsText);
}

void UHungerBar::setCurrentFood()
{
	CurrentFood = GameMode->getCurrentFoodsInChest();
	FText intAsText = FText::AsNumber(CurrentFood);
	CurrentHungerLabel->SetText(intAsText);
}

void UHungerBar::setProgressBar()
{
	float nb = CurrentFood / MaxFood;
	HungerBar->SetPercent(nb);
}

void UHungerBar::OnFoodStocked()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, TEXT("Called"));
	//CurrentHungerLabel = SurvivalGameState. FoodCountInChest
}

void UHungerBar ::NativeOnInitiliazed()
{
	Super::OnInitialized();

	Chest = Cast<AChest>(UGameplayStatics::GetActorOfClass(GetWorld(), AChest::StaticClass()));
	Chest->HungerBar_OnFoodStocked.AddUObject(this, &UHungerBar::OnFoodStocked); //see above in wiki
}

