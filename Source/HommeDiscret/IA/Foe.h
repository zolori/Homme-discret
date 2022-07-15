// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Character.h"
#include "Foe.generated.h"

class AFood;
class AFoodSpot;
class UCapsuleComponent;
class USphereComponent;
class AStealthGameMode;
class AAIC_Foe;
UCLASS()
class HOMMEDISCRET_API AFoe : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFoe();
	UPROPERTY(VisibleAnywhere)
		float SpaceBetween;
	UPROPERTY(VisibleAnywhere)
	AAIC_Foe* PawnController;

protected:
	AStealthGameMode* GameMode;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AActor> FoeController;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* InteractionSphere;
	UPROPERTY(VisibleAnywhere)
		float InteractionSphereRadius = 200.0f;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* CollisionSphere;
	UPROPERTY(VisibleAnywhere)
		float CollisionSphereRadius = 150.0f;

	UCharacterMovementComponent* CharacMov;
	UPROPERTY(VisibleAnywhere)
		float FoeSpeed;
	UPROPERTY(VisibleAnywhere)
		float FoeSpeedHolding;
	//Food Parameters
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly,Category = "Food")
		UStaticMeshComponent* FoodMesh;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AFood* FoodRef;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		AFoodSpot* FoodSpotNearby;
	UPROPERTY(VisibleAnywhere)
		bool HaveToDroppedFood;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		bool IsHoldingFood;

	//Food spawn parameters
	UPROPERTY(VisibleAnywhere)
		UWorld* CurrentWorld;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> FoodClass;

	FActorSpawnParameters SpawnInfo;
	FVector SpawnLocation;
	FRotator SpawnRotation;

	virtual void BeginPlay() override;

public:
	UFUNCTION()
		void CallbackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
		void CallbackComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	bool PickUpFood();
	FVector DropFoodOnTheFloor();
	bool DropFoodInFoodSpot();
	void InstantiateFood();
	bool GetHaveToDroppedFood();
	void SetHaveToDroppedFood(bool Value);
	bool GetHoldingFood();

	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
