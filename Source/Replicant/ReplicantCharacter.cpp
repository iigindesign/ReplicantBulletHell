#include "ReplicantCharacter.h"

#include "Components/CapsuleComponent.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"

#include "Components/StaticMeshComponent.h"


AReplicantCharacter::AReplicantCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
			
	MaxHealth = 100.0f;
	CurrentHealth = MaxHealth;
	bIsImmortal = true;
}

// Replicated Properties
void AReplicantCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AReplicantCharacter, CurrentHealth);
}

void AReplicantCharacter::OnHealthUpdate()
{
	// Client
	if (IsLocallyControlled())
	{
		// FString healthMessage = FString::Printf(TEXT("%f health remainingggg"), CurrentHealth);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);

		if (CurrentHealth <= 0)
		{
			// FString deathMessage = FString::Printf(TEXT("%f DEATH"));
			// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, deathMessage);
		}
	}

	// Server
	if (GetLocalRole() == ROLE_Authority)
	{
		// FString healthMessage = FString::Printf(TEXT("Server: %s now has %f health"), *GetFName().ToString(), CurrentHealth);
		// GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, healthMessage);
		if (!bIsImmortal && CurrentHealth <= 0)
		{
			Destroy();
		}
	}

	// Methods for all machines
}

void AReplicantCharacter::OnRep_CurrentHealth()
{
	OnHealthUpdate();
}

void AReplicantCharacter::SetCurrentHealth(float healthValue)
{
	if (GetLocalRole() == ROLE_Authority)
	{
		CurrentHealth = FMath::Clamp(healthValue, 0.f, MaxHealth);
		OnHealthUpdate();
	}
}

float AReplicantCharacter::TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float damageApplied = CurrentHealth - DamageTaken;
	SetCurrentHealth(damageApplied);
	return damageApplied;
}
