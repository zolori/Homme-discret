// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_PickUpFood.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HOMMEDISCRET_API UBTTask_PickUpFood : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_PickUpFood(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);
	
};
