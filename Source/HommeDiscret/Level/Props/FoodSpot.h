// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FoodSpot.generated.h"

class AFood;
class USphereComponent;
UCLASS()
class HOMMEDISCRET_API AFoodSpot : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFoodSpot();
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* PlateMesh;
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* FoodMesh;
	UPROPERTY(VisibleAnywhere)
		USphereComponent* CollisionSphere;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		float CollisionSphereRadius=100.0f;
	
	UWorld* CurrentWorld;
	FVector SpawnLocation;
	FActorSpawnParameters SpawnInfo;
	FRotator SpawnRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		TSubclassOf<AActor> FoodClass;

	UPROPERTY(VisibleAnywhere)
		AFood* FoodRef;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	bool FillFoodSpot(AFood* NewFood);
	void EmptyFoodSpot();
	void InstantiateFoodSpot();
	FVector GetRealLocation();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
