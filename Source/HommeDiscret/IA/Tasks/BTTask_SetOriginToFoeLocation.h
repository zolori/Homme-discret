// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_SetOriginToFoeLocation.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HOMMEDISCRET_API UBTTask_SetOriginToFoeLocation : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SetOriginToFoeLocation(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);
};
