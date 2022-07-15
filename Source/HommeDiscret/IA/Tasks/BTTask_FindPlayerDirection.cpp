// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerDirection.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "HommeDiscret/IA/AIC_Foe.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "HommeDiscret/IA/Tasks/BB_keys.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"


UBTTask_FindPlayerDirection::UBTTask_FindPlayerDirection(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("FindPlayerDirection");
}

EBTNodeResult::Type UBTTask_FindPlayerDirection::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AAIC_Foe>(owner_comp.GetAIOwner());
	UBlackboardComponent* Blackboard = cont->GetBlackboardComponent();
	APawn* FoePawn = cont->GetPawn();
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	FVector const playerLocation = player->GetActorLocation();

	//Finding Direction
	FVector newLocation = playerLocation - Blackboard->GetValueAsVector(bb_keys::LastPlayerLocation);

	//Adding origin location and distance
	newLocation += FoePawn->GetActorLocation();
	//DrawDebugSphere(GetWorld(), newLocation, 50.0f, 32, FColor::Magenta, false, 5.0f);

	Blackboard->SetValueAsVector(bb_keys::target_location, newLocation);
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
