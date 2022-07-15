// Fill out your copyright notice in the Description page of Project Settings.


#include "FoeSpawnerRoom.h"
#include "HommeDiscret/IA/NavigationPoint.h"
#include "Kismet/GameplayStatics.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"

// Sets default values
AFoeSpawnerRoom::AFoeSpawnerRoom()
{
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("Static Mesh"));
	RootComponent = StaticMesh;

	TileFloorHolder = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloorHolder"));
	TileFloorHolder->SetupAttachment(RootComponent);

	TileWallHolder = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWallHolder"));
	TileWallHolder->SetupAttachment(RootComponent);

	TileFloor_0 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_0"));
	TileFloor_0->SetupAttachment(TileFloorHolder);
	TileFloor_0->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X - SizeOfMesh, TileFloorHolder->GetRelativeLocation().Y, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_1 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_1"));
	TileFloor_1->SetupAttachment(TileFloorHolder);
	TileFloor_1->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X - SizeOfMesh, TileFloorHolder->GetRelativeLocation().Y + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Z))
		;
	TileFloor_2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_2"));
	TileFloor_2->SetupAttachment(TileFloorHolder);
	TileFloor_2->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X - (SizeOfMesh * 2), TileFloorHolder->GetRelativeLocation().Y, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_3 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_3"));
	TileFloor_3->SetupAttachment(TileFloorHolder);
	TileFloor_3->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X - (SizeOfMesh * 2), TileFloorHolder->GetRelativeLocation().Y + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_4 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_4"));
	TileFloor_4->SetupAttachment(TileFloorHolder);
	TileFloor_4->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X, TileFloorHolder->GetRelativeLocation().Y + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_5 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_5"));
	TileFloor_5->SetupAttachment(TileFloorHolder);
	TileFloor_5->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Y, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_6 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_6"));
	TileFloor_6->SetupAttachment(TileFloorHolder);
	TileFloor_6->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Y + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_7 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_7"));
	TileFloor_7->SetupAttachment(TileFloorHolder);
	TileFloor_7->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X + (SizeOfMesh * 2), TileFloorHolder->GetRelativeLocation().Y, TileFloorHolder->GetRelativeLocation().Z));

	TileFloor_8 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileFloor_8"));
	TileFloor_8->SetupAttachment(TileFloorHolder);
	TileFloor_8->SetRelativeLocation(FVector(TileFloorHolder->GetRelativeLocation().X + (SizeOfMesh * 2), TileFloorHolder->GetRelativeLocation().Y + SizeOfMesh, TileFloorHolder->GetRelativeLocation().Z));

	TileWall_0 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_0"));
	TileWall_0->SetupAttachment(TileWallHolder);
	TileWall_0->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X + SizeOfMesh, TileWallHolder->GetRelativeLocation().Y - Offset, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_1 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_1"));
	TileWall_1->SetupAttachment(TileWallHolder);
	TileWall_1->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X + (SizeOfMesh * 2), TileWallHolder->GetRelativeLocation().Y - Offset, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_2 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_2"));
	TileWall_2->SetupAttachment(TileWallHolder);
	TileWall_2->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X + (SizeOfMesh * 2) +  Offset, TileWallHolder->GetRelativeLocation().Y, TileWallHolder->GetRelativeLocation().Z + Offset));
	TileWall_2->SetRelativeRotation(FRotator(0.f, YawRotation, 0.f));

	TileWall_3 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_3"));
	TileWall_3->SetupAttachment(TileWallHolder);
	TileWall_3->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X - SizeOfMesh, TileWallHolder->GetRelativeLocation().Y + Offset + SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_4 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_4"));
	TileWall_4->SetupAttachment(TileWallHolder);
	TileWall_4->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X - (SizeOfMesh * 2), TileWallHolder->GetRelativeLocation().Y + Offset + SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_5 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_5"));
	TileWall_5->SetupAttachment(TileWallHolder);
	TileWall_5->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X - Offset - (SizeOfMesh * 2), TileWallHolder->GetRelativeLocation().Y + SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));
	TileWall_5->SetRelativeRotation(FRotator(0.f, YawRotation, 0.f));

	TileWall_6 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_6"));
	TileWall_6->SetupAttachment(TileWallHolder);
	TileWall_6->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X - Offset - (SizeOfMesh * 2), TileWallHolder->GetRelativeLocation().Y, TileWallHolder->GetRelativeLocation().Z + Offset));
	TileWall_6->SetRelativeRotation(FRotator(0.f, YawRotation, 0.f));

	TileWall_7 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_7"));
	TileWall_7->SetupAttachment(TileWallHolder);
	TileWall_7->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X - (SizeOfMesh * 2), TileWallHolder->GetRelativeLocation().Y - Offset, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_8 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_8"));
	TileWall_8->SetupAttachment(TileWallHolder);
	TileWall_8->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X - SizeOfMesh, TileWallHolder->GetRelativeLocation().Y - Offset, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_9 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_9"));
	TileWall_9->SetupAttachment(TileWallHolder);
	TileWall_9->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X + (SizeOfMesh * 2) + Offset, TileWallHolder->GetRelativeLocation().Y+ SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));
	TileWall_9->SetRelativeRotation(FRotator(0.f, YawRotation, 0.f));

	TileWall_10 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_10"));
	TileWall_10->SetupAttachment(TileWallHolder);
	TileWall_10->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X + (SizeOfMesh * 2), TileWallHolder->GetRelativeLocation().Y + Offset + SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_11 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_11"));
	TileWall_11->SetupAttachment(TileWallHolder);
	TileWall_11->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X + SizeOfMesh, TileWallHolder->GetRelativeLocation().Y + Offset + SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileWall_12 = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileWall_12"));
	TileWall_12->SetupAttachment(TileWallHolder);
	TileWall_12->SetRelativeLocation(FVector(TileWallHolder->GetRelativeLocation().X, TileWallHolder->GetRelativeLocation().Y + Offset + SizeOfMesh, TileWallHolder->GetRelativeLocation().Z + Offset));

	TileArch = CreateDefaultSubobject<UStaticMeshComponent>(FName("TileArch"));
	TileArch->SetupAttachment(RootComponent);
	TileArch->SetRelativeLocation(FVector(RootComponent->GetRelativeLocation().X, RootComponent->GetRelativeLocation().Y - Offset, RootComponent->GetRelativeLocation().Z));
	TileArch->SetWorldScale3D(FVector(0.5f, 0.5f, 0.5f));

	Params.Owner = this;
}

// Called when the game starts or when spawned
void AFoeSpawnerRoom::BeginPlay()
{
	Super::BeginPlay();
	ExitingPointActor = GetWorld()->SpawnActor<ANavigationPoint>(ExitingPoint, FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y - (SizeOfMesh * 2), RootComponent->GetComponentLocation().Z + 100.0f), FRotator::ZeroRotator, Params);
	if (ExitingPointActor != nullptr)
	{
		//UE_LOG(LogTemp, Warning, TEXT("SpawnedFoe not null"));
		ExitingPointActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}

	StartingPointActor = GetWorld()->SpawnActor<ANavigationPoint>(StartingPoint, FVector(RootComponent->GetComponentLocation().X, RootComponent->GetComponentLocation().Y + (SizeOfMesh * 2), RootComponent->GetComponentLocation().Z + 100.0f), FRotator::ZeroRotator, Params);
	if (StartingPointActor != nullptr)
	{
			//UE_LOG(LogTemp, Warning, TEXT("SpawnedFoe not null"));
			StartingPointActor->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
	}
}

