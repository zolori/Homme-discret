// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FillFoodSpot.h"

UBTTask_FillFoodSpot::UBTTask_FillFoodSpot(FObjectInitializer const& object_initializer) 
{
	NodeName = TEXT("Fill Food Spot");
}


EBTNodeResult::Type UBTTask_FillFoodSpot::ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory)
{
	return EBTNodeResult::Type();
}
