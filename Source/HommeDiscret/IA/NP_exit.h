// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "NavigationPoint.h"
#include "Components/SphereComponent.h"
#include "NP_exit.generated.h"


/**
 * 
 */
UCLASS()
class HOMMEDISCRET_API ANP_exit : public ANavigationPoint
{
	GENERATED_BODY()
	

public :
	ANP_exit();

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* CollisionSphere;

};
