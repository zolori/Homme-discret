// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_ResetBlackboardKey.h"
#include "HommeDiscret/IA/AIC_Foe.h"

UBTTask_ResetBlackboardKey::UBTTask_ResetBlackboardKey(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Reset BB Key");
}


EBTNodeResult::Type UBTTask_ResetBlackboardKey::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	AAIC_Foe* FoeController = Cast<AAIC_Foe>(owner_comp.GetAIOwner());
	UBlackboardComponent* Blackboard = FoeController->get_blackboard();
	Blackboard->ClearValue(GetSelectedBlackboardKey());
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
