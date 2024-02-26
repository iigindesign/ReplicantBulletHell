// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantNaiveProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerState.h"

// This projectile will only spawn for the client that fired - has inertia applied.

AReplicantNaiveProjectile::AReplicantNaiveProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;
	SetReplicateMovement(false);

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(ProjectileRoot);

	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));


	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Cosmetic Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileRoot);

	ProjectileMovementComponent->InitialSpeed = 50.0f;
	ProjectileMovementComponent->MaxSpeed = 50.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;
}

void AReplicantNaiveProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void AReplicantNaiveProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
