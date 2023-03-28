// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ReplicantProjectile.generated.h"

UCLASS()
class REPLICANT_API AReplicantProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AReplicantProjectile();  

    // Sphere component used to test collision.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
    class USphereComponent* SphereComponent;

    // Static Mesh used to provide a visual representation of the object.
    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
    class UStaticMeshComponent* StaticMesh;

    // Movement component for handling projectile movement. REPLICATED?
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
    class UProjectileMovementComponent* ProjectileMovementComponent;

    //The damage type and damage that will be done by this projectile
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage)
    TSubclassOf<class UDamageType> DamageType;

    //The damage dealt by this projectile.
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Damage)
    float Damage;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    virtual void Destroyed() override;
     
    UFUNCTION(Category = Projectile)
    virtual void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

    // Particle used when the projectile impacts against another object and explodes.
    UPROPERTY(EditAnywhere, Category = Effects)
        class UParticleSystem* ExplosionEffect;

    UParticleSystem* DestroyedEffect = nullptr;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
