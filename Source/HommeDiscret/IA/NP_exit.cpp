// Fill out your copyright notice in the Description page of Project Settings.


#include "NP_exit.h"
#include "Foe.h"

ANP_exit::ANP_exit()
{
	
	CollisionSphere = CreateDefaultSubobject<USphereComponent>(FName("SphereComponent"));
	RootComponent = CollisionSphere;

}

void ANP_exit::BeginPlay()
{
	Super::BeginPlay();
	CollisionSphere->SetCollisionProfileName(FName("OverlapAll"));
}