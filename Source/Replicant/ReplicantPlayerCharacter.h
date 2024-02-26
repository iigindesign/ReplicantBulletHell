#pragma once

#include "CoreMinimal.h"
#include "ReplicantCharacter.h"
#include "ReplicantPlayerCharacter.generated.h"

/**
 * 
 */
UCLASS()
class REPLICANT_API AReplicantPlayerCharacter : public AReplicantCharacter
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

	// ---------------------- Set in Blueprint to have the Projectile_BPs --------------------------

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AReplicantProjectile> ProjectileClass;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AReplicantCosmeticProjectile> CosmeticProjectileClass;

	UPROPERTY(EditAnywhere, Category = Projectile)
	TSubclassOf<class AReplicantNaiveProjectile> NaiveProjectileClass;

	UPROPERTY(EditAnywhere, Category = Gameplay)
	TSubclassOf<class ABlastDeflectAbility> AbilityClass;

public:
	AReplicantPlayerCharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* ShootAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* BlastDeflectAction;

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle FiringTimer;
	/** A timer handle used for providing the fire rate delay in-between spawns.*/
	FTimerHandle AbilityTimer;

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleFire();

	UFUNCTION(NetMulticast, Reliable)
	void HandleFireMulticast();

	void SpawnCosmeticProjectile();

	void SpawnProjectile();

	/** Server function for spawning projectiles.*/
	UFUNCTION(Server, Reliable)
	void HandleAbility();

	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartFire();

	/** Function for ending weapon fire. Once this is called, the player can use StartFire again. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopFire();

	/** Delay between shots in seconds. Used to control fire rate for your test projectile, but also to prevent an overflow of server functions from binding SpawnProjectile directly to input.*/
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float FireRate;

	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float ProjectileSpawnVerticalOffset;
	UPROPERTY(EditDefaultsOnly, Category = "Gameplay")
	float ProjectileSpawnHorizontalOffset;


	/** If true, you are in the process of firing projectiles. */
	bool bIsFiringWeapon;

	/** If true, you are in the process of firing projectiles. */
	bool bIsUsingAbility;

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Function for beginning weapon fire.*/
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StartAbility();

	/** Function for ending weapon fire. Once this is called, the player can use StartFire again. */
	UFUNCTION(BlueprintCallable, Category = "Gameplay")
	void StopAbility();

};
