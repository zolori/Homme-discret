// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Chest.generated.h"

DECLARE_MULTICAST_DELEGATE(FHungerBar);

class AStealthGameMode;
UCLASS()
class HOMMEDISCRET_API AChest : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AChest();

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* BaseStaticMesh;

	UPROPERTY(VisibleAnywhere, Category = "Mesh")
		UStaticMeshComponent* TopStaticMesh;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereCollision;

	AStealthGameMode* GameMode;

	UPROPERTY(VisibleAnywhere)
		float SphereRadius;

	UPROPERTY(VisibleAnywhere)
		int NumberFoodsContained;

	UPROPERTY(VisibleAnywhere)
		int MaxFoodsToWin;

	FHungerBar HungerBar_OnFoodStocked;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddingFood(int FoodValue);

	int GetNumberFoodsContained();

	int GetMaxFoodsToWin();


};
