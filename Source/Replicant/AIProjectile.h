// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ReplicantProjectile.h"
#include "AIProjectile.generated.h"

/**
 * 
 */
UCLASS()
class REPLICANT_API AAIProjectile : public AReplicantProjectile
{
	GENERATED_BODY()
	protected:

    virtual void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
};
