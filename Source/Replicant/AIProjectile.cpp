// Fill out your copyright notice in the Description page of Project Settings.


#include "AIProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "ReplicantPlayerCharacter.h"
#include "ReplicantAI.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"

void AAIProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (Cast<AAIProjectile>(OtherActor) || Cast<AReplicantAI>(OtherActor))
		{
			return;
		}
		// This calls TakeDamage() on the actor it collided with
		if (Cast<AReplicantPlayerCharacter>(OtherActor))
		{
			AReplicantAI* AIOwner = Cast<AReplicantAI>(GetOwner());
			if (AIOwner)
			{
				UGameplayStatics::ApplyPointDamage(OtherActor, Damage, OtherActor->GetActorForwardVector(), SweepResult, AIOwner->Controller, this, DamageType);
			}
			DestroyedEffect = ExplosionEffect;
		}
		else
		{
			DestroyedEffect = nullptr;
		}
	}


	Destroy();
}
