// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindPlayerLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "HommeDiscret/IA/AIC_Foe.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "HommeDiscret/IA/Tasks/BB_keys.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "GameFramework/Character.h"
#include "DrawDebugHelpers.h"


UBTTask_FindPlayerLocation::UBTTask_FindPlayerLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Player Location");
}

EBTNodeResult::Type UBTTask_FindPlayerLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//get player character and the npc's controller
	ACharacter* const player = UGameplayStatics::GetPlayerCharacter(GetWorld(), 0);
	auto const cont = Cast<AAIC_Foe>(owner_comp.GetAIOwner());

	//get player location to use as an origin
	FVector const playerLocation = player->GetActorLocation();

	if (first)
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::LastPlayerLocation, playerLocation);
		//DrawDebugSphere(GetWorld(), player->GetActorLocation(), 50.0f, 32, FColor::Red, false, 5.0f);
	}
	else 
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::target_location, playerLocation);
		//DrawDebugSphere(GetWorld(), player->GetActorLocation(), 50.0f, 32, FColor::Orange, false, 5.0f);
	}


	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
