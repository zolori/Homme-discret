// Fill out your copyright notice in the Description page of Project Settings.

#include "Chest.h"
#include "Kismet/GameplayStatics.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"

// Sets default values
AChest::AChest()
{
	PrimaryActorTick.bCanEverTick = true;
	BaseStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Base StaticMesh"));
	RootComponent = BaseStaticMesh;
	TopStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Top StaticMesh"));
	TopStaticMesh->SetupAttachment(BaseStaticMesh);
	SphereRadius = 80.0f;
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT(" Sphere collision"));
	SphereCollision->InitSphereRadius(SphereRadius);
	SphereCollision->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AChest::BeginPlay()
{
	Super::BeginPlay();
	GameMode = Cast<AStealthGameMode>(UGameplayStatics::GetGameMode(GetWorld()));

	MaxFoodsToWin = 5;
}

// Called every frame
void AChest::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AChest::AddingFood(int FoodValue)
{
	GameMode->AddFoodInChest(FoodValue);
	HungerBar_OnFoodStocked.Broadcast();
}

int AChest::GetNumberFoodsContained()
{
	return NumberFoodsContained;
}

int AChest::GetMaxFoodsToWin()
{
	return MaxFoodsToWin;
}
