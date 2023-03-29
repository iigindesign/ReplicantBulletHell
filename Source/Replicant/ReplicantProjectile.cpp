// Fill out your copyright notice in the Description page of Project Settings.


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
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;

	////definition for the spherecomponent that will serve as the root component for the projectile and its collision.
	//spherecomponent = createdefaultsubobject<uspherecomponent>(text("sphererootcomponent"));
	//spherecomponent->initsphereradius(15.5f);
	//spherecomponent->setcollisionprofilename(text("overlapalldynamic"));
	//rootcomponent = spherecomponent;

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = ProjectileRoot;

	//Definition for the Mesh that will serve as your visual representation.
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(RootComponent);
	StaticMesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	StaticMesh->SetGenerateOverlapEvents(true);

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
		StaticMesh->OnComponentBeginOverlap.AddDynamic(this, &AReplicantProjectile::OnProjectileImpact);
	}
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

// TODO: NEED TO FIGURE OUT AI CONTROLLER is this an implicit RPC?
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