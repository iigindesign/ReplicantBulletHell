// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicantNaiveProjectile.generated.h"

UCLASS()
class REPLICANT_API AReplicantNaiveProjectile : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AReplicantNaiveProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class USceneComponent* ProjectileRoot;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class USphereComponent* SphereComponent;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
		class UStaticMeshComponent* StaticMesh;

	// Movement component for handling projectile movement. REPLICATED?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
		class UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
