// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BehaviorTreeTypes.h"
#include "BTTask_SetOriginToRoomCenter.generated.h"

/**
 * 
 */
UCLASS(Blueprintable)
class HOMMEDISCRET_API UBTTask_SetOriginToRoomCenter : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
public:
	UBTTask_SetOriginToRoomCenter(FObjectInitializer const& object_initializer);
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& owner_comp, uint8* node_memory);
};
