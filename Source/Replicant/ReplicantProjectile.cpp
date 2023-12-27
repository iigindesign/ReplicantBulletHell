#include "ReplicantProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/DamageType.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
AReplicantProjectile::AReplicantProjectile()
{
}

// Called when the game starts or when spawned
void AReplicantProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReplicantProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// called on actor destroyed, actor destructions replicate 
void AReplicantProjectile::Destroyed()
{
	FVector spawnLocation = GetActorLocation();
	if (DestroyedEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyedEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	}
}

void AReplicantProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (GetInstigator())
		{
			// This calls TakeDamage() on the actor it collided with
			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, OtherActor->GetActorForwardVector(), SweepResult, GetInstigator()->Controller, this, DamageType);
			DestroyedEffect = ExplosionEffect;
		}
		else
		{
			DestroyedEffect = nullptr;
		}
	}
	

	Destroy();
}

void AReplicantProjectile::SetProjectileVelocity_Implementation(FVector NewVelocity)
{
	if (ProjectileMovementComponent)
	{
		ProjectileMovementComponent->Velocity = NewVelocity;
		ProjectileMovementComponent->UpdateComponentVelocity();
	}
}