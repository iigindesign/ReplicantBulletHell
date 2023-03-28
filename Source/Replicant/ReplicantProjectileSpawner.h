// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicantProjectileSpawner.generated.h"

UCLASS()
class REPLICANT_API AReplicantProjectileSpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	AReplicantProjectileSpawner();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* StaticMesh;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Rotating Movement")
	class URotatingMovementComponent* RotatingMovement;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawner")
	class UReplicantProjectileSpawnerComp* ProjectileSpawner;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
