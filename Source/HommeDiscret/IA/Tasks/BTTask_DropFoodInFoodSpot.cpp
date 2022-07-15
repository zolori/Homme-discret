// Fill out your copyright notice in the Description page of Project Settings.

#include "BTTask_DropFoodInFoodSpot.h"
#include "HommeDiscret/IA/AIC_Foe.h"
#include "HommeDiscret/IA/Foe.h"

UBTTask_DropFoodInFoodSpot::UBTTask_DropFoodInFoodSpot(FObjectInitializer const& object_initializer)
{
	NodeName = TEXT("Drop Food In A Food Spot");
}

EBTNodeResult::Type UBTTask_DropFoodInFoodSpot::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{

	AAIC_Foe* FoeController = Cast<AAIC_Foe>(owner_comp.GetAIOwner());
	AFoe* Foe = Cast<AFoe>(FoeController->GetCharacter());
	UBlackboardComponent* Blackboard = FoeController->GetBlackboardComponent();
	bool HaveDroppedFood = Foe->DropFoodInFoodSpot();
	EBTNodeResult::Type HaveSucceeded = EBTNodeResult::Succeeded;
	if (HaveDroppedFood==false)
	{	
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, TEXT("Drop Food in Foodspot has failed !!!"));
		HaveSucceeded = EBTNodeResult::Failed;
	}
	FinishLatentTask(owner_comp, HaveSucceeded);
	return HaveSucceeded;
}
