#include "PlayerProjectile.h"
#include "Kismet/GameplayStatics.h"
#include "ReplicantPlayerCharacter.h"
#include "ReplicantAI.h"
#include "GameFramework/Controller.h"
#include "GameFramework/Pawn.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Engine/EngineTypes.h"
#include "AIProjectile.h"

// Projectile spawned on server and simulated individually on each client

APlayerProjectile::APlayerProjectile()
{
	PrimaryActorTick.bCanEverTick = true;

	bReplicates = false;
	SetReplicateMovement(false);

	ProjectileRoot = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovementComponent->SetUpdatedComponent(ProjectileRoot);
	// TODO: ...these should probably be set in the BP
	ProjectileMovementComponent->InitialSpeed = 50.0f;
	ProjectileMovementComponent->MaxSpeed = 50.0f;
	ProjectileMovementComponent->bRotationFollowsVelocity = true;
	ProjectileMovementComponent->ProjectileGravityScale = 0.0f;

	DamageType = UDamageType::StaticClass();
	Damage = 10.0f;

	// Physical Component
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere Component"));
	SphereComponent->SetupAttachment(ProjectileRoot);

	SphereComponent->InitSphereRadius(15.5f);
	SphereComponent->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	SphereComponent->SetGenerateOverlapEvents(true);

	ProjectileVisualComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Projectile Mesh Visual"));
	ProjectileVisualComponent->SetupAttachment(ProjectileRoot);

	// TODO: commented out destruction to avoid dealing w collisions just yet
	// SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &APlayerProjectile::OnProjectileImpact);

}

void APlayerProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void APlayerProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// FVector Location = GetActorLocation();

	// Print the location to the screen
	// GEngine->AddOnScreenDebugMessage(-1, 0.1f, FColor::Red, FString::Printf(TEXT("Projectile is at: %s"), *Location.ToString()));
}

// TODO: ok so something is wrong with how we are setting the static mesh dynamically ...but given up on figuring out why it doesnt show...
// Also the init speed is wrong.
// OMG
// 
// the issue wasnt even with the dynamic additoin??????? i think it's with adding the static mesh FUCKk
// 
// wtFFFFF
// 
//void APlayerProjectile::BeginPlay()
//{
//	if (GetNetMode() == ENetMode::NM_Client)
//	{
//		if (ProjectileMesh)
//		{
//
//			ProjectileVisualComponent = NewObject<UStaticMeshComponent>(this);
//			ProjectileVisualComponent->RegisterComponent();
//			ProjectileVisualComponent->AttachToComponent(ProjectileRoot, FAttachmentTransformRules::KeepWorldTransform);
//			ProjectileVisualComponent->SetStaticMesh(ProjectileMesh);
//			FString RoleText = FString::Printf(TEXT("mesh?...: %d"), bool(ProjectileVisualComponent->GetStaticMesh()));
//			GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, RoleText);
//
//		}
//	}
//}

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
