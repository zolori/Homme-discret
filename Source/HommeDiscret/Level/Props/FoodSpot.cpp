// Fill out your copyright notice in the Description page of Project Settings.


#include "FoodSpot.h"
#include "Food.h"
// Sets default values
AFoodSpot::AFoodSpot()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	RootComponent = StaticMesh;

	PlateMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Plate Mesh"));
	PlateMesh->SetupAttachment(RootComponent);

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Food Mesh"));
	FoodMesh->SetupAttachment(PlateMesh);
	FoodMesh->SetRelativeLocation(PlateMesh->GetRelativeLocation());

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("Collision Sphere"));
	CollisionSphere->SetupAttachment(RootComponent);
	CollisionSphere->SetSphereRadius(CollisionSphereRadius);
}

// Called when the game starts or when spawned
void AFoodSpot::BeginPlay()
{
	Super::BeginPlay();
	CurrentWorld = GetWorld();
	SpawnLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z - 150.0f);
    SpawnRotation = this->GetActorRotation();
}

bool AFoodSpot::FillFoodSpot(AFood* NewFood)
{
	bool Return = false;
	if (NewFood != nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Fill %s"), *NewFood->GetName()));
		FoodRef = NewFood;
		FoodRef->Hide();
		FoodMesh->SetStaticMesh(FoodRef->GetStaticMeshUsed());
		Return = true;
	}
	return Return; 
}

void AFoodSpot::EmptyFoodSpot()
{
	FoodMesh->SetStaticMesh(nullptr);
	FoodRef = nullptr;
}

void AFoodSpot::InstantiateFoodSpot()
{
	AActor* Actor = CurrentWorld->SpawnActor(FoodClass, &SpawnLocation, &SpawnRotation, SpawnInfo);
	AFood* FoodActor = Cast<AFood>(Actor);
	FillFoodSpot(FoodActor);
}

FVector AFoodSpot::GetRealLocation()
{
	FVector NewVector = this->GetActorLocation();
	NewVector.X = NewVector.X + CollisionSphere->GetScaledSphereRadius() - 5.0f;
	return NewVector;
}

// Called every frame
void AFoodSpot::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

