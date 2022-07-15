// Fill out your copyright notice in the Description page of Project Settings.


#include "BBTTask_FindRandomLocation.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "HommeDiscret/IA/AIC_Foe.h"
//#include "BehaviorTree/Blackboard/Blackboard/BlackboardKeyType.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "HommeDiscret/IA/Tasks/BB_keys.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "HommeDiscret/IA/NavigationPoint.h"

UBBTTask_FindRandomLocation::UBBTTask_FindRandomLocation(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Random Location");
}



EBTNodeResult::Type UBBTTask_FindRandomLocation::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	//get AI Controller and its NPC
	auto const cont = Cast<AAIC_Foe>(owner_comp.GetAIOwner());
	//auto const npc = cont->GetPawn();

	//obtain npc location to use as an origin location
	//FVector const origin = npc->GetActorLocation();
	FNavLocation loc;

	//get the navigation system and generate a random location on the NavMesh
	UNavigationSystemV1* const nav_sys = UNavigationSystemV1::GetCurrent(GetWorld());
	UBlackboardComponent* BlackBoard = cont->get_blackboard();

	if (nav_sys->GetRandomPointInNavigableRadius(BlackBoard->GetValueAsVector(bb_keys::target_location), search_radius, loc, nullptr))
	{
		BlackBoard->SetValueAsVector(bb_keys::target_location, loc.Location);
	}

	//finish with success
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
