// Fill out your copyright notice in the Description page of Project Settings.


#include "Foe.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "HommeDiscret/Level/Props/Food.h"
#include "HommeDiscret/Level/Props/FoodSpot.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"
#include "AIC_Foe.h"
//#include "FoeSpawner.h"
#include "NavigationPoint.h"

// Sets default values
AFoe::AFoe()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	//HaveToDroppedFood = false;
	AIControllerClass = FoeController;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	CharacMov = GetCharacterMovement();

	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionSphere"));
	InteractionSphere->SetSphereRadius(InteractionSphereRadius);
	InteractionSphere->SetupAttachment(RootComponent);

	CollisionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("CollisionSphere"));
	CollisionSphere->SetSphereRadius(CollisionSphereRadius);
	CollisionSphere->SetupAttachment(RootComponent);

	FoodMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("FoodMesh"));
	FoodMesh->SetSimulatePhysics(false);
	FoodMesh->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	FoeSpeed = CharacMov->MaxWalkSpeed;
	FoeSpeedHolding = FoeSpeed/2;
	FoodRef = nullptr;
	FoodSpotNearby = nullptr;
	HaveToDroppedFood = false;
	IsHoldingFood = false;
	GameMode = Cast<AStealthGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}
// Called when the game starts or when spawned
void AFoe::BeginPlay()
{
	Super::BeginPlay();
	if (GetMesh()->DoesSocketExist("FoodSocket"))
	{
		//UE_LOG(LogTemp, Warning, TEXT("Exist!"));
		FoodMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("FoodSocket"));
		FoodMesh->SetRelativeScale3D(FVector(0.05f, 0.05f, 0.05f));
		//SetupSphereTrace();
	}
	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &AFoe::CallbackComponentBeginOverlap);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &AFoe::CallbackComponentEndOverlap);
	CurrentWorld=GetWorld();
	SpawnLocation = FVector(this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z - 300.0f);
	SpawnRotation = this->GetActorRotation();
	PawnController = Cast<AAIC_Foe>(GetController());
	//InstantiateFood();
}

void AFoe::CallbackComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->IsA(AFood::StaticClass()))
	{
		if (!IsHoldingFood)
		{
			AFood* NewFood = Cast<AFood>(OtherActor);
			if (NewFood != nullptr)
			{
				//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, FString::Printf(TEXT("Food is near %s"), *NewFood->GetName()));
				FoodRef = NewFood;
			}
		}
	}
	else if (OtherActor->IsA(AFoodSpot::StaticClass()))
	{
		if (IsHoldingFood)
		{
			AFoodSpot* NewFoodSpot = Cast<AFoodSpot>(OtherActor);
			if (NewFoodSpot->FoodRef == nullptr)
			{
				FoodSpotNearby = NewFoodSpot;
			}
		}
	}
	else if (OtherActor->IsA(ANavigationPoint::StaticClass()))
	{
		if (PawnController != nullptr)
		{
			//GameMode->RemoveFoeInRoom();
			GameMode->SetAIWaiting(this);
			//this->SetActorLocation(GameMode->GetNavLocation(0));	
			SetHaveToDroppedFood(false);
			PawnController->StopAIBehavior();

		}
	}
}

void AFoe::CallbackComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor->IsA(AFood::StaticClass()))
	{
		if (!IsHoldingFood)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Purple, TEXT("Food is not here"));
			FoodRef = nullptr;
		}
	}
	else if (OtherActor->IsA(AFoodSpot::StaticClass()))
	{
		FoodSpotNearby = nullptr;
	}
}

bool AFoe::PickUpFood()
{
	bool Return = false;
	if (FoodRef != nullptr)
	{
		IsHoldingFood = true;
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("Foe PickUp %s"),*FoodRef->GetName()));
		FoodMesh->SetStaticMesh(FoodRef->GetStaticMeshUsed());
		FoodRef->Hide();
		Return = true;
		CharacMov->MaxWalkSpeed = FoeSpeedHolding;
	}
	return Return;
}

FVector AFoe::DropFoodOnTheFloor()
{
	FVector NewVector = FVector(0, 0, 0);
	if (FoodRef != nullptr)
	{
		FoodRef->Show(this->GetActorLocation(), this->GetActorForwardVector());
		NewVector = FoodRef->GetRealLocation();
		FoodRef = nullptr;
		IsHoldingFood = false;
		FoodMesh->SetStaticMesh(nullptr);
		CharacMov->MaxWalkSpeed = FoeSpeed;
	}
	return NewVector;
}

bool AFoe::DropFoodInFoodSpot()
{
	bool Return = false;
	if (FoodSpotNearby != nullptr)
	{
		if (FoodRef != nullptr)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("FoodRef %s"), *FoodRef->GetName()));
			bool Success = FoodSpotNearby->FillFoodSpot(FoodRef);
			if (Success == true)
			{
				GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, FString::Printf(TEXT("Foe have filled the spot with  %s"), *FoodRef->GetName()));
				IsHoldingFood = false;
				FoodMesh->SetStaticMesh(nullptr);
				FoodRef = nullptr;
				Return = true;
				CharacMov->MaxWalkSpeed = FoeSpeed;
			}
		}
	}
	return Return;
}

void AFoe::InstantiateFood()
{
	if (FoodClass != nullptr)
	{
		if (!IsHoldingFood)
		{
			GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, FString::Printf(TEXT("Foe Class %s"), *FoodClass->GetName()));
			//AActor* Actor = CurrentWorld->SpawnActor(FoodClass, &SpawnLocation, &SpawnRotation, SpawnInfo);
			AActor* Actor = GetWorld()->SpawnActor<AFood>(FoodClass, SpawnLocation, SpawnRotation, SpawnInfo);
			if (Actor != nullptr)
			{
				AFood* NewFood = Cast<AFood>(Actor);
				if (NewFood != nullptr)
				{
					GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Magenta, TEXT("SetHaveToDroppedFood to TRUE"));
					SetHaveToDroppedFood(true);
					FoodRef = NewFood;
					PickUpFood();
				}
			}
		}
	}
	else 
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("FoodClass is not defined"));
	}
}

bool AFoe::GetHaveToDroppedFood()
{
	return HaveToDroppedFood;
}

void AFoe::SetHaveToDroppedFood(bool Value)
{
	HaveToDroppedFood = Value;
	PawnController->BBHaveToDroppedFood(HaveToDroppedFood);
}

bool AFoe::GetHoldingFood()
{
	return IsHoldingFood;
}

// Called every frame
void AFoe::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AFoe::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}
