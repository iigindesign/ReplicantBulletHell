// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantCosmeticProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"

AReplicantCosmeticProjectile::AReplicantCosmeticProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// This projectile will only spawn for the client that fired - has inertia applied.
	bReplicates = false;
	SetReplicateMovement(true);

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ProjectileRoot;

	// This handles the actual collisions
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(RootComponent);

	//it'd be the best if there's some sort of ini or blueprint on the sizing etc. that both the functional and cosmetic projectile references
	SphereComponent->InitSphereRadius(15.5f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetGenerateOverlapEvents(true);

	// Should this be called on every client? 
	// FIXME: ALSO! I don't think our client can just send some cosmetics over to the other clients. I think we NEED to go through the server
	// server will likely have to spawn the projectile on the other clients at a delay...
	// The fact that we are setting bReplicates to true...when it's only called on the local client...dont think it would do anything...
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AReplicantCosmeticProjectile::OnProjectileImpact);
	
	//Definition for the Mesh that will serve as your visual representation.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);


	//Definition for the Projectile Movement Component.
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Cosmetic Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileRoot);
	ProjectileMovementComponent->InitialSpeed = 200.0f;
	ProjectileMovementComponent->MaxSpeed = 1500.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

// Called when the game starts or when spawned
void AReplicantCosmeticProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReplicantCosmeticProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AReplicantCosmeticProjectile::OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor && OtherActor != this)
	{
		if (GetInstigator())
		{
			// DestroyedEffect = ExplosionEffect;
		}
		else
		{
			// DestroyedEffect = nullptr;
		}
		Destroy();
	}
}

// called on actor destroyed, actor destructions replicate 
void AReplicantCosmeticProjectile::Destroyed()
{
	if (DestroyedEffect)
	{
		FVector spawnLocation = GetActorLocation();
		UGameplayStatics::SpawnEmitterAtLocation(this, DestroyedEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	}
}