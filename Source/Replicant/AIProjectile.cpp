#include "AIProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "ReplicantPlayerCharacter.h"
#include "ReplicantAI.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Ability/BlastDeflectAbility.h"
#include "GameFramework/ProjectileMovementComponent.h"

AAIProjectile::AAIProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicateMovement(true);

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ProjectileRoot;

	//Definition for the Mesh that will serve as your visual representation.
	ProjectileVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	ProjectileVisualComponent->SetupAttachment(RootComponent);
	ProjectileVisualComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	ProjectileVisualComponent->SetGenerateOverlapEvents(true);

	////hard coding transformation of shape sphere
	//StaticMesh->SetRelativeLocation(FVector(0.0f, 0.0f, -37.5f));
	//StaticMesh->SetRelativeScale3D(FVector(0.75f, 0.75f, 0.75f));

	//Definition for the Projectile Movement Component.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileRoot);
	ProjectileMovementComponent->InitialSpeed = 1500.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	//TODO: understand AddDynamic
	//Registering the Projectile Impact function on a Hit event.
	if (GetLocalRole() == ROLE_Authority)
	{
		ProjectileVisualComponent->OnComponentBeginOverlap.AddDynamic(this, &AAIProjectile::OnProjectileImpact);
	}
}

void AAIProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor)
	{
		if (Cast<AAIProjectile>(OtherActor) || Cast<AReplicantAI>(OtherActor) || Cast<ABlastDeflectAbility>(OtherActor))
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
