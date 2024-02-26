// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantCosmeticProjectile.h"
#include "Components/SceneComponent.h"
#include "Components/SphereComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/PlayerState.h"

// This projectile will only spawn for the client that fired - has inertia applied.

AReplicantCosmeticProjectile::AReplicantCosmeticProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;
	SetReplicateMovement(false);

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	// This handles the actual collisions
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(ProjectileRoot);

	//it'd be the best if there's some sort of ini or blueprint on the sizing etc. that both the functional and cosmetic projectile references
	SphereComponent->InitSphereRadius(15.5f);
	//SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));
	// SphereComponent->SetGenerateOverlapEvents(true);
	// SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AReplicantCosmeticProjectile::OnProjectileImpact);
	
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	StaticMesh->SetupAttachment(ProjectileRoot);
	StaticMesh->SetCollisionProfileName(TEXT("NoCollision"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Projectile Cosmetic Movement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileRoot);
	
	ProjectileSpeed = 50.0f;
	MockLatency = 1100.0f;

	const FRotator Rotation = ProjectileRoot->GetComponentRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);

	InertiaTotalTime = MockLatency/1000; //in s
	InertiaElapsedTime = 0.0f;

	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

}

void AReplicantCosmeticProjectile::BeginPlay()
{
	Super::BeginPlay();
	StartingLocation = GetActorLocation();
	// This is the predicted location in 500ms after the user fires, at 250ms the position would be at 000 for server
	PredictedLocation = GetActorLocation() + GetActorForwardVector() * ProjectileSpeed * MockLatency / 2 / 1000;
}

void AReplicantCosmeticProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// If still inertia time (latency halved), interpolate to initial predicted position
	if (InertiaElapsedTime < InertiaTotalTime)
	{

		float Alpha = FMath::Clamp(InertiaElapsedTime / InertiaTotalTime, 0.0f, 1.0f);
		FVector NewPosition = FMath::Lerp(StartingLocation, PredictedLocation, Alpha); // Interpolate the position
		UE_LOG(LogTemp, Warning, TEXT("Elapsed Time: %f; DeltaTime: %f; Alpha: %f; NewPosition: %s"), InertiaElapsedTime, DeltaSeconds, Alpha, *NewPosition.ToString());

		SetActorLocation(NewPosition); // Set the new position
	}
	else if (!Interpolated)
	{
		Interpolated = true; // ensures the velocity is only set once
		ProjectileMovementComponent->Velocity = GetActorForwardVector() * ProjectileSpeed;
	}
	InertiaElapsedTime += DeltaSeconds;
	
	float ping = Cast<APlayerController>(GetOwner()->GetInstigatorController())->PlayerState->GetPingInMilliseconds();

	FString RoleText = FString::Printf(TEXT("Ping: %f"), ping);
	GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, RoleText);

	//UE_LOG(LogTemp, Warning, TEXT("Ping: %d"), ping);
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