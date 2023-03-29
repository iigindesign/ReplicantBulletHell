// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicantAI.h"
#include "RotatingAI.generated.h"

/**
 * 
 */
UCLASS()
class REPLICANT_API ARotatingAI : public AReplicantAI
{
	GENERATED_BODY()
	ARotatingAI();

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotating Movement")
	class URotatingMovementComponent* RotatingMovement;

};
