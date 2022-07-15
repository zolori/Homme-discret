// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "AIC_Foe.generated.h"

/**
 * 
 */
class AFoodSpot;
class AStealthGameMode;
UCLASS()
class HOMMEDISCRET_API AAIC_Foe : public AAIController
{
	GENERATED_BODY()
	
public:
	//AAIC_Foe(FObjectInitializer const& object_initializer = FObjectInitializer::Get());
	AAIC_Foe();

	UPROPERTY(VisibleAnywhere)
	bool HasAlreadyStartedBT;

	void BeginPlay() override;
	void OnPossess(APawn* const pawn) override;
	void StartAIBehavior();
	void InstantiateFoodToFoe(bool HaveFood);
	void StopAIBehavior();
	void BBHaveToDroppedFood(bool Value);
	class UBlackboardComponent* get_blackboard() const;

	bool GetBehaviorTreeIsRunning();

	FVector GetEnterLocation();

	FVector  GetExitLocation();

	FVector GetOriginLocation();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		TArray<AFoodSpot*> FoodSpots;

	AFoodSpot* GetOneRandomFoodSpot();

private:
	UPROPERTY(VisibleAnywhere)
		AStealthGameMode* GameMode;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category="AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AI", meta = (AllowPrivateAccess = "true"))
	class UBehaviorTree* Btree;

	class UBlackboardComponent* Blackboard;

	class UAISenseConfig_Sight* SightConfig;

	UFUNCTION()
		void OnTargetDetected(AActor* actor, FAIStimulus const stimulus);
		void SetupPerceptionSystem();
		//void FindFoodSpots();
};
