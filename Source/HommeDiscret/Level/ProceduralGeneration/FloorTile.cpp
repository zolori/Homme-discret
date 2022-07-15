// Fill out your copyright notice in the Description page of Project Settings.


#include "FloorTile.h"

// Sets default values
AFloorTile::AFloorTile()
{

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	RootComponent = StaticMesh;
}

// Called when the game starts or when spawned
void AFloorTile::BeginPlay()
{
	Super::BeginPlay();
	
}

FVector AFloorTile::GetCoordinate()
{
	return this->GetActorLocation();
}
