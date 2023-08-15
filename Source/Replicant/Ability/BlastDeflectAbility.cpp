// Fill out your copyright notice in the Description page of Project Settings.


#include "BlastDeflectAbility.h"
#include "Components/SphereComponent.h"
#include "../ReplicantProjectile.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABlastDeflectAbility::ABlastDeflectAbility()
{
	bReplicates = true;
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BlastDeflectAbilitySphere = CreateDefaultSubobject<USphereComponent>(TEXT("AbilitySphere"));
	RootComponent = BlastDeflectAbilitySphere;

	BlastDeflectAbilitySphere->SetGenerateOverlapEvents(true);
	BlastDeflectAbilitySphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
}

// Called when the game starts or when spawned
void ABlastDeflectAbility::BeginPlay()
{
	FVector spawnLocation = GetActorLocation();
	if (BlastEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(this, BlastEffect, spawnLocation, FRotator::ZeroRotator, true, EPSCPoolMethod::AutoRelease);
	}

	Super::BeginPlay();
	SetLifeSpan(LifeSpan);

	BlastDeflectAbilitySphere->OnComponentBeginOverlap.AddDynamic(this, &ABlastDeflectAbility::SphereBeginOverlap);
	BlastDeflectAbilitySphere->OnComponentEndOverlap.AddDynamic(this, &ABlastDeflectAbility::SphereEndOverlap);

}

// Called every frame
void ABlastDeflectAbility::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABlastDeflectAbility::SphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AReplicantProjectile* projectile = Cast<AReplicantProjectile>(OtherActor);
	if (projectile)
	{
		bool check = bFromSweep;
		
		FVector myLocation = GetActorLocation();
		FVector computedNormal = projectile->GetActorLocation() - myLocation;

		FVector endpoint = computedNormal + projectile->GetActorLocation();

		DrawDebugLine(GetWorld(), projectile->GetActorLocation(), endpoint, FColor::Blue, true, 0, 0, 10.f);

		float speed = (projectile->ProjectileMovementComponent->Velocity).Size();
		computedNormal.Normalize();
		FVector newVelocity = computedNormal *speed*1.2f;

		HandleUpdateProjectile(projectile, newVelocity);
	}
}

void ABlastDeflectAbility::HandleUpdateProjectile_Implementation(AReplicantProjectile* Projectile, FVector NewVelocity)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		Projectile->SetProjectileVelocity(NewVelocity);
		UE_LOG(LogTemp, Warning, TEXT("SERVER, %s"), *NewVelocity.ToString());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Client, %s"), *NewVelocity.ToString());
	}
}

//void ABlastDeflectAbility::HandleUpdateProjectile(AReplicantProjectile* Projectile, FVector NewVelocity)
//{
//	Projectile->SetProjectileVelocity(NewVelocity);
//	
//}

void ABlastDeflectAbility::SphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{

}
