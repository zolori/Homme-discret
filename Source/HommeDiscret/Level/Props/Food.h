// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "HommeDiscret/Character/Hero.h"
#include "Food.generated.h"

UCLASS()
class HOMMEDISCRET_API AFood : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFood();

protected:
	UPROPERTY(VisibleAnywhere)
		UStaticMeshComponent* StaticMesh;

	UPROPERTY(VisibleAnywhere)
		USphereComponent* SphereCollider;

	UPROPERTY(VisibleAnywhere)
		float UnderFloorPosition = 100.0f;
	UPROPERTY(VisibleAnywhere)
		float DropOnFloorPosition = 60.0f;

	UPROPERTY(VisibleAnywhere)
		float InFrontPosition = 100.0f;

	UPROPERTY(VisibleAnywhere)
		int FoodValue = 1;

	UPROPERTY(VisibleAnywhere)
		float InRate = 0.25f;

	FTimerHandle Timer;
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	void Hide();
	void Show(FVector DropActorPos, FVector DropActorForward);
	FVector GetRealLocation();
	void SetPhysics();
	int GetFoodValue();
	UStaticMesh* GetStaticMeshUsed();
};
