// Fill out your copyright notice in the Description page of Project Settings.


#include "Food.h"
#include "GameFramework/Character.h"

// Sets default values
AFood::AFood()
{
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	RootComponent = StaticMesh;
	
	SphereCollider = CreateDefaultSubobject<USphereComponent>(FName("SphereCollider"));
	SphereCollider->SetupAttachment(RootComponent);
	SphereCollider->InitSphereRadius(60.0f);
	StaticMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void AFood::BeginPlay()
{
	Super::BeginPlay();
}

void AFood::Hide()
{
	this->StaticMesh->SetSimulatePhysics(false);
	this->SetActorLocation(FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z - UnderFloorPosition));
}

void AFood::Show(FVector DropActorPos, FVector DropActorForward)
{
	FVector newPos = FVector(DropActorPos + DropActorForward * InFrontPosition);
	newPos.Z = DropOnFloorPosition;
	this->SetActorLocation(newPos);
	this->SetActorRotation(FQuat(0.0f, 0.0f, 0.0f, 0.0f));
	GetWorldTimerManager().SetTimer(Timer, this, &AFood::SetPhysics, InRate, false);
}

FVector AFood::GetRealLocation()
{
	FVector NewVector;
	NewVector = this->GetActorLocation();
	NewVector.X = NewVector.X + SphereCollider->GetScaledSphereRadius()-5.0f;
	return NewVector;
}

void AFood::SetPhysics()
{
	StaticMesh->SetSimulatePhysics(true);
}

int AFood::GetFoodValue()
{
	return FoodValue;
}

UStaticMesh* AFood::GetStaticMeshUsed()
{
	return StaticMesh->GetStaticMesh();
}
