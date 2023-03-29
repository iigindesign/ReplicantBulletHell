// Fill out your copyright notice in the Description page of Project Settings.
#include "RotatingAI.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "Components/SceneComponent.h"
#include "ReplicantProjectileSpawnerComp.h"

ARotatingAI::ARotatingAI()
{
	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->SetUpdatedComponent(Cast<USceneComponent>(ProjectileSpawner));
}
