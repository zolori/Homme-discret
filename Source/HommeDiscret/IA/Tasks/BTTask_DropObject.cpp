// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_DropObject.h"
#include "HommeDiscret/IA/AIC_Foe.h"
#include "HommeDiscret/IA/Foe.h"
#include "HommeDiscret/IA/Tasks/BB_keys.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"
#include "BehaviorTree/BlackboardComponent.h"

UBTTask_DropObject::UBTTask_DropObject(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Drop Food On The Floor");
}


EBTNodeResult::Type UBTTask_DropObject::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	AAIC_Foe* FoeController = Cast<AAIC_Foe>(owner_comp.GetAIOwner());

	AFoe* Foe = Cast<AFoe>(FoeController->GetCharacter());
	UBlackboardComponent* Blackboard = FoeController->GetBlackboardComponent();
	FVector LastFoodLocation = Foe->DropFoodOnTheFloor();
	if (LastFoodLocation.IsZero() == false)
	{
		Blackboard->SetValueAsVector(bb_keys::LastFoodLocation, LastFoodLocation);
	}
	FinishLatentTask(owner_comp, EBTNodeResult::Succeeded);
	return EBTNodeResult::Succeeded;
}
