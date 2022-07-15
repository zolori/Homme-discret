// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "HommeDiscret/Character/Hero.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"


#include "HDAnimInstance.generated.h"

/**
 * 
 */
UCLASS(transient, Blueprintable, hideCategories = AnimInstance, BlueprintType)
class HOMMEDISCRET_API UHDAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UCharacterMovementComponent* MovementPtr;
	AStealthGameMode* GameMode;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

public:
	UHDAnimInstance();
	AHero* Hero;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		float Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsMoving;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsHolding;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsInHand;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool IsHitted;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool GameOver;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Generic")
		bool Won;


	UFUNCTION(BlueprintCallable, Category = "MyAnim")
		void AnimNotify_End(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable, Category = "MyAnim")
		void AnimNotify_Begin(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable, Category = "MyAnim")
		void AnimNotify_HitBegin(UAnimNotify* Notify);

	UFUNCTION(BlueprintCallable, Category = "MyAnim")
		void AnimNotify_HitEnd(UAnimNotify* Notify);

	

protected:
	virtual void NativeBeginPlay() override;
};
