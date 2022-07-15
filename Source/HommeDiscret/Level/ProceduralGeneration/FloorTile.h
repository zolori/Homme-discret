// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FloorTile.generated.h"

UCLASS()
class HOMMEDISCRET_API AFloorTile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloorTile();
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	bool bIsHoldingCrate = false;
	bool bIsHoldingMultipleCrates = false;
	bool bIsHoldingFoodSpot = false;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	FVector GetCoordinate();
};
