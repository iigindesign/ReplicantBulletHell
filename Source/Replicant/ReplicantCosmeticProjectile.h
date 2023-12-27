// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicantCosmeticProjectile.generated.h"

UCLASS()
class REPLICANT_API AReplicantCosmeticProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReplicantCosmeticProjectile();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USceneComponent* ProjectileRoot;


	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class USphereComponent* SphereComponent;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* StaticMesh;

	// Particle used when the projectile impacts against another object and explodes.
	UPROPERTY(EditAnywhere, Category = Effects)
	class UParticleSystem* ExplosionEffect;

	UParticleSystem* DestroyedEffect = nullptr;


	// Movement component for handling projectile movement. REPLICATED?
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	class UProjectileMovementComponent* ProjectileMovementComponent;

protected:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void Destroyed() override;

	UFUNCTION(Category = Projectile)
	void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

};
