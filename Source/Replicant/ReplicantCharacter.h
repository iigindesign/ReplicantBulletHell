// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "ReplicantCharacter.generated.h"


UCLASS(config=Game)
class AReplicantCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	AReplicantCharacter();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* StaticMesh;

protected:				
	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	bool bIsImmortal;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	// triggered on client when server replicates value to it
	UFUNCTION()
	void OnRep_CurrentHealth();

	void OnHealthUpdate();

public:
	UFUNCTION(BlueprintPure, Category=Health)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
	UFUNCTION(BlueprintPure, Category=Health)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = Health)
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = Health)
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamagerCauser) override;
};

