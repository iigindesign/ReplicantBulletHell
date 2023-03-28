// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "ReplicantProjectileSpawnerComp.generated.h"


UCLASS(Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class REPLICANT_API UReplicantProjectileSpawnerComp : public USceneComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UReplicantProjectileSpawnerComp();

	UPROPERTY(EditAnywhere, Category = "Projectile Spawner")
	int SpawnCount;

	UPROPERTY(EditAnywhere, Category = "Projectile Spawner")
	float SpawnerRadius;

	UPROPERTY(EditAnywhere, Category = "Projectile Spawner")
	float SpawnRateInSec;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Gameplay|Projectile")
	TSubclassOf<class AReplicantProjectile> ProjectileClass;

	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void SpawnProjectile();

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;
		
};
