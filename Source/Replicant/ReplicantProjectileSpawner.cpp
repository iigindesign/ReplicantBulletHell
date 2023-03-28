// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantProjectileSpawner.h"
#include "GameFramework/RotatingMovementComponent.h"
#include "ReplicantProjectileSpawnerComp.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AReplicantProjectileSpawner::AReplicantProjectileSpawner()
{
	bReplicates = true;

	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawner = CreateDefaultSubobject<UReplicantProjectileSpawnerComp>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(RootComponent);

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ReplicantStaticMesh"));
	StaticMesh->SetupAttachment(RootComponent);

	RotatingMovement = CreateDefaultSubobject<URotatingMovementComponent>(TEXT("RotatingMovement"));
	RotatingMovement->SetUpdatedComponent(ProjectileSpawner);
}

// Called when the game starts or when spawned
void AReplicantProjectileSpawner::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AReplicantProjectileSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

