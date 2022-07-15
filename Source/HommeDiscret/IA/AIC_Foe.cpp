// Fill out your copyright notice in the Description page of Project Settings.


#include "AIC_Foe.h"
#include "HommeDiscret/Character/Hero.h"
#include "NavigationPoint.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Perception/AIPerceptionStimuliSourceComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "HommeDiscret/IA/Tasks/BB_keys.h"
#include "HommeDiscret/Level/Props/FoodSpot.h"
#include "Foe.h"
#include "GameFramework/Character.h"
#include "HommeDiscret/Tools/GameMode/StealthGameMode.h"

AAIC_Foe::AAIC_Foe()
{
    BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTree"));
    Blackboard = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComp"));
    SetupPerceptionSystem();
}



void AAIC_Foe::BeginPlay()
{       
    Super::BeginPlay();
    HasAlreadyStartedBT = false;
    GameMode = Cast<AStealthGameMode>(UGameplayStatics::GetGameMode(GetWorld()));
}

void AAIC_Foe::OnPossess(APawn* const pawn)
{
    Super::OnPossess(pawn);
    if (Blackboard)
    {
        Blackboard->InitializeBlackboard(*Btree->BlackboardAsset);
    }
}

void AAIC_Foe::StartAIBehavior()
{
    if (HasAlreadyStartedBT)
    {
        UE_LOG(LogTemp, Warning, TEXT("RestartLogic"));
        BehaviorTreeComponent->RestartTree();
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("StartLogic"));
        RunBehaviorTree(Btree);
        BehaviorTreeComponent->StartTree(*Btree);
        HasAlreadyStartedBT = true;
    }
}

void AAIC_Foe::InstantiateFoodToFoe(bool HaveFood)
{
    if (HaveFood)
    {
        AFoe* Foe = Cast<AFoe>(GetPawn());
        if (Foe != nullptr)
        {
            UE_LOG(LogTemp, Warning, TEXT("Foe with Food"));
            Foe->InstantiateFood();
        }
    }
}

void AAIC_Foe::StopAIBehavior()
{
    UE_LOG(LogTemp, Warning, TEXT("StopLogic"));
    BehaviorTreeComponent->StopTree(EBTStopMode::Safe);
    Blackboard->ClearValue(bb_keys::ExitLocation);
}

void AAIC_Foe::BBHaveToDroppedFood(bool Value)
{
    Blackboard->SetValueAsBool(bb_keys::HaveToDroppedFood, Value);
}

UBlackboardComponent* AAIC_Foe::get_blackboard() const 
{
    return Blackboard;
}

void AAIC_Foe::OnTargetDetected(AActor* actor, FAIStimulus const stimulus)
{
    if (auto const ch = Cast<AHero>(actor))
    {
        get_blackboard()->SetValueAsBool(bb_keys::player_onSight, stimulus.WasSuccessfullySensed());
    }
}


void AAIC_Foe::SetupPerceptionSystem()
{
    SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
    SetPerceptionComponent(*CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("Perception Component")));
    SightConfig->SightRadius = 500.0f;
    SightConfig->LoseSightRadius = SightConfig->SightRadius + 25.0f;
    SightConfig->PeripheralVisionAngleDegrees = 90.0f;
    SightConfig->SetMaxAge(5.0f);
    SightConfig->AutoSuccessRangeFromLastSeenLocation = 520.0f;
    SightConfig->DetectionByAffiliation.bDetectEnemies = true;
    SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
    SightConfig->DetectionByAffiliation.bDetectNeutrals = true;
     // add sight configuration component to perception component
     GetPerceptionComponent()->SetDominantSense(*SightConfig->GetSenseImplementation());
     GetPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAIC_Foe::OnTargetDetected);
     GetPerceptionComponent()->ConfigureSense(*SightConfig);
}


FVector AAIC_Foe::GetEnterLocation()
{

    return GameMode->GetEnterLocation();
}

FVector AAIC_Foe::GetExitLocation()
{
    return GameMode->GetExitLocation();
}

bool AAIC_Foe::GetBehaviorTreeIsRunning()
{
    bool Result = true;
    if (BehaviorTreeComponent->IsRunning() == false)
    {
        Result = false;
    }
    return Result;
}

FVector AAIC_Foe::GetOriginLocation()
{
    return GameMode->GetOriginLocation();
}


AFoodSpot* AAIC_Foe::GetOneRandomFoodSpot()
{
    return GameMode->GetOneRandomFoodSpot();
}