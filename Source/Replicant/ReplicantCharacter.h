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

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;
	
	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;              


public:
	AReplicantCharacter();
	
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Static Mesh used to provide a visual representation of the object.
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
	class UStaticMeshComponent* StaticMesh;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

protected:
	UPROPERTY(EditAnywhere, Category = "Gameplay|Projectile")
	TSubclassOf<class AReplicantProjectile> ProjectileClass;

	/** Delay between shots in seconds. Used to control fire rate for your test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	/** If true, you are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** Function for ending weapon fire. Once this is called, the player can use StartFire again. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;

	///////////////////////////////////////////////////////////////////////////////////

	/** Called for movement input */
	void Move(const FInputActionValue& Value);
			
	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	
	// To add mapping context
	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditDefaultsOnly, Category = Health)
	float MaxHealth;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentHealth)
	float CurrentHealth;

	// triggered on client when server replicates value to it
	UFUNCTION()
	void OnRep_CurrentHealth();

	void OnHealthUpdate();

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	UFUNCTION(BlueprintPure, Category=Health)
	FORCEINLINE float GetMaxHealth() const { return MaxHealth; }
	
	UFUNCTION(BlueprintPure, Category=Health)
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }

	UFUNCTION(BlueprintCallable, Category = Health)
	void SetCurrentHealth(float healthValue);

	UFUNCTION(BlueprintCallable, Category = Health)
	float TakeDamage(float DamageTaken, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamagerCauser) override;
};

