// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AISense_Sight.h"
#include "GameFramework/Character.h"
#include "Hero.generated.h"


class AFoodSpot;
class AFood;
class AChest;
class USpringArmComponent;
class USphereComponent;
class UStaticMeshComponent;
class AStealthGameMode;
UCLASS()
class HOMMEDISCRET_API AHero : public ACharacter
{
	GENERATED_BODY()
private:
	AStealthGameMode* GameMode;

public:
	// Sets default values for this character's properties
	AHero();

	UPROPERTY(VisibleAnywhere)
		USphereComponent* CollisionSphere;

	UPROPERTY(VisibleAnywhere)
		float CollisionSphereRadius = 200.0f;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
		UCameraComponent* FollowCamera;

	UCharacterMovementComponent* CharacMov;

	UPROPERTY(VisibleAnywhere, Category = "Food")
		UStaticMeshComponent* FoodMesh;

	UPROPERTY(VisibleAnywhere, Category = "Food")
		bool  IsHoldingFood;

	UPROPERTY(VisibleAnywhere, Category = "Food")
		AFood* FoodRef;

	UPROPERTY(VisibleAnywhere, Category = "Food")
		AChest* ChestNearby;

	UPROPERTY(VisibleAnywhere, Category = "Food")
		AFoodSpot* FoodSpotNearby;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float HeroSpeed;

	UPROPERTY(VisibleAnywhere, Category = "Movement")
		float HeroSpeedHolding;

	//Movement + Zoom
	void MoveForward(float Axis);
	void MoveRight(float Axis);
	void Zoom(float value);

	//Show Pause menu
	void PauseMenu();

	//PickUp object
	void DropObject();
	void PickUpObject(AFood* NewFood);
	void Interact();
	bool bDead;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION()
		void CallbackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		void CallbackComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:

	class UAIPerceptionStimuliSourceComponent* Stimulus;
	void SetupStimulus();
};
