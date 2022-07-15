// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FindExit.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "HommeDiscret/IA/AIC_Foe.h"
#include "HommeDiscret/IA/Tasks/BB_keys.h"


UBTTask_FindExit::UBTTask_FindExit(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Find Exit");
}


EBTNodeResult::Type UBTTask_FindExit::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	auto const cont = Cast<AAIC_Foe>(owner_comp.GetAIOwner());

	if (cont != nullptr)
	{
		cont->get_blackboard()->SetValueAsVector(bb_keys::ExitLocation, cont->GetExitLocation());
	}
	//finish with success
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}

