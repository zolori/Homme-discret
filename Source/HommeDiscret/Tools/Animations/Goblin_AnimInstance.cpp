// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin_AnimInstance.h"
#include "HommeDiscret/IA/Foe.h"

UGoblin_AnimInstance::UGoblin_AnimInstance()
{
	Speed = 0;
	IsMoving = false;
	IsHolding = false;
	IsInHand = false;
}

void UGoblin_AnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
	AFoe* Foe = Cast<AFoe>(GetOwningActor());
	//AActor* Foe = GetOwningActor();

	if (Foe != nullptr) {
		Speed = Foe->GetVelocity().Size();
		IsHolding = Foe->GetHoldingFood();
	}

	if (Speed > 0)
		IsMoving = true;
	else
		IsMoving = false;

	if (!IsHolding)
		IsInHand = false;
}

void UGoblin_AnimInstance::AnimNotify_End(UAnimNotify* Notify)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("end"));
	//MovementPtr->MaxWalkSpeed = Hero->HeroSpeed;
	IsInHand = true;
}

void UGoblin_AnimInstance::AnimNotify_Begin(UAnimNotify* Notify)
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, TEXT("begin"));
	//MovementPtr->MaxWalkSpeed = 0;
}