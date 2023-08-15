// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BlastDeflectAbility.generated.h"

UCLASS()
class REPLICANT_API ABlastDeflectAbility : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABlastDeflectAbility();

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
		float LifeSpan;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Collision)
		class USphereComponent* BlastDeflectAbilitySphere;
	
	// Particle used when the projectile impacts against another object and explodes.
	UPROPERTY(EditAnywhere, Category = Effects)
		class UParticleSystem* BlastEffect;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(Server, Reliable)
	void HandleUpdateProjectile(AReplicantProjectile* Projectile, FVector NewVelocity);

	//UFUNCTION()
	//void HandleUpdateProjectile(AReplicantProjectile* Projectile, FVector NewVelocity);

	UFUNCTION()
	void SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

};
