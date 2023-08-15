// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "ReplicantPlayerCharacter.h"
#include "ReplicantAI.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AIProjectile.h"

//TODO: NEED TO FIGURE OUT BETTER SYSTEM TO ACCOUNT FOR PROJECTILE TYPE IGNORANCE
void APlayerProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherComp)
	{
		//TODO: this prob should be in a data class
		FName IgnoreProjectileTag = "IgnoreProjectile";
		if (OtherComp->ComponentHasTag(IgnoreProjectileTag))
		{
			return;
		}
	}
	if (OtherActor)
	{
		if (Cast<AReplicantPlayerCharacter>(OtherActor) || Cast<APlayerProjectile>(OtherActor))
		{
			return;
		}
 		if (Cast<AReplicantAI>(OtherActor) || Cast<AAIProjectile>(OtherActor))
		{
			DestroyedEffect = ExplosionEffect;

			UGameplayStatics::ApplyPointDamage(OtherActor, Damage, OtherActor->GetActorForwardVector(), SweepResult, GetInstigator()->Controller, this, DamageType);
		}
		else
		{
			DestroyedEffect = nullptr;
		}
	}

	Destroy();
}
