// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "HommeDiscret/Level/Props/Chest.h"
#include "Hideout.generated.h"

UCLASS()
class HOMMEDISCRET_API AHideout : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AHideout();
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	//Do no put static mesh here; Hold the floor
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileFloorHolder;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileFloor_0;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileFloor_1;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileFloor_2;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileFloor_3;

	//Do no put static mesh here; Hold the wall
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWallHolder;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_0;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_1;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_2;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_3;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_4;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_5;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_6;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_7;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileWall_8;

	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* TileArch;

	UPROPERTY(EditAnywhere)
		TSubclassOf<AChest> Chest;
		

	float SizeOfMesh = 200.f;
	float Offset = 100.f;
	float YawRotation = 90.f;
	FActorSpawnParameters Params;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
};
