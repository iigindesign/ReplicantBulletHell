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


// Sets default values
APlayerProjectile::APlayerProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = true;
	SetReplicateMovement(true);

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ProjectileRoot;

	//Definition for the Projectile Movement Component.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileRoot);
	// TODO: ...these should probably be set in the BP
	ProjectileMovementComponent->InitialSpeed = 200.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	if (GetLocalRole() == ROLE_Authority)
	{
		// Functional Sphere Collision Detection on Server
		SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
		SphereComponent->SetupAttachment(RootComponent);

		SphereComponent->InitSphereRadius(15.5f);
		SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
		SphereComponent->SetGenerateOverlapEvents(true);

		//Registering the Projectile Impact function on a Hit event.
		SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerProjectile::OnProjectileImpact);
	}
	else
	{
		// Cosmetics on simulated proxies
		if (ProjectileMesh)
		{
			ProjectileVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh Visual"));
			ProjectileVisualComponent->SetupAttachment(RootComponent);
			ProjectileVisualComponent->SetStaticMesh(ProjectileMesh);
		}
	}
}


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
