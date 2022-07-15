// Fill out your copyright notice in the Description page of Project Settings.


#include "HDAnimInstance.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"

UHDAnimInstance::UHDAnimInstance()
{
	GameMode = Cast<AStealthGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
	GameOver = false;
	Won = false;
	Speed = 0;
	Speed = 0;
	IsMoving = false;
	IsHolding = false;
	IsInHand = false;
	IsHitted = false;
}

void UHDAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	Hero = Cast<AHero>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	MovementPtr = Cast<UCharacterMovementComponent>(Hero->GetCharacterMovement());
}

void UHDAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AActor* OwningActor = GetOwningActor();

	if (OwningActor != nullptr)
		Speed = OwningActor->GetVelocity().Size();

	if (Speed > 0)
		IsMoving = true;
	else
		IsMoving = false;

	if (Hero != nullptr)
		IsHolding = Hero->IsHoldingFood;

	if (!IsHolding)
		IsInHand = false;

	if (GameMode != nullptr)
	{
		if (GameMode->GetGameOver())
		{
			GameOver = true;
			MovementPtr->MaxWalkSpeed = 0;
		}

		if (GameMode->GetWon())
		{
			Won = true;
			MovementPtr->MaxWalkSpeed = 0;
		}
	}
}

void UHDAnimInstance::AnimNotify_End(UAnimNotify* Notify)
{
	MovementPtr->MaxWalkSpeed = Hero->HeroSpeedHolding;
	IsInHand = true;
}

void UHDAnimInstance::AnimNotify_Begin(UAnimNotify* Notify)
{
	MovementPtr->MaxWalkSpeed = 0;
}

void UHDAnimInstance::AnimNotify_HitEnd(UAnimNotify* Notify)
{
	MovementPtr->MaxWalkSpeed = Hero->HeroSpeed / 2;
	IsHitted = true;
}

void UHDAnimInstance::AnimNotify_HitBegin(UAnimNotify* Notify)
{
	MovementPtr->MaxWalkSpeed = 0;
}
