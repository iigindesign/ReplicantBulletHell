// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantPlayerCharacter.h"

#include "Camera/CameraComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"

#include "ReplicantProjectile.h"
#include "ReplicantNaiveProjectile.h"
#include "ReplicantCosmeticProjectile.h"

#include "Ability/BlastDeflectAbility.h"
#include "GameFramework/PlayerState.h"

AReplicantPlayerCharacter::AReplicantPlayerCharacter()
{
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement  7
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	// GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 800.0f; // The camera follows at this distance behind the character	
	CameraBoom->SetUsingAbsoluteRotation(true); // Don't want arm to rotate when character does
	CameraBoom->SetRelativeRotation(FRotator(-60.f, 0.f, 0.f));
	CameraBoom->bDoCollisionTest = false; // Don't want to pull camera in when it collides with level

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	//Initialize fire rateStaticCl
	FireRate = 0.1f;
	bIsFiringWeapon = false;

	ProjectileSpawnHorizontalOffset = 100.0f;
	ProjectileSpawnVerticalOffset = 50.0f;
}

void AReplicantPlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
}

void AReplicantPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//APlayerController* controller = Cast<APlayerController>(Controller);
	//if (controller)
	//{
	//	APlayerState* playerstate = controller->GetPlayerState<APlayerState>();
	//	if (playerstate)
	//	{
	//		float ping = playerstate->GetPingInMilliseconds();
	//		UE_LOG(LogTemp, Warning, TEXT("Ping: %d, Role: %d"), ping, GetLocalRole());
	//	}
	//}


}

void AReplicantPlayerCharacter::SpawnProjectile()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * ProjectileSpawnHorizontalOffset) + (GetActorUpVector() * ProjectileSpawnVerticalOffset);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	//Instigator is the actor Class causing this to happen       
	spawnParameters.Instigator = GetInstigator();
	//Owner is the Controller that spawned the instigator
	spawnParameters.Owner = this;

	AReplicantProjectile* spawnedProjectile = GetWorld()->SpawnActor<AReplicantProjectile>(ProjectileClass, spawnLocation, spawnRotation, spawnParameters);
}

void AReplicantPlayerCharacter::SpawnCosmeticProjectile()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * ProjectileSpawnHorizontalOffset) + (GetActorUpVector() * ProjectileSpawnVerticalOffset);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	//Instigator is the actor Class causing this to happen       
	spawnParameters.Instigator = GetInstigator();
	//Owner is the Controller  that spawned the instigator
	spawnParameters.Owner = this;

	AReplicantNaiveProjectile* spawnedNaiveProjectile = GetWorld()->SpawnActor<AReplicantNaiveProjectile>(NaiveProjectileClass, spawnLocation, spawnRotation, spawnParameters);
	AReplicantCosmeticProjectile* spawnedProjectile = GetWorld()->SpawnActor<AReplicantCosmeticProjectile>(CosmeticProjectileClass, spawnLocation, spawnRotation, spawnParameters);
}

void AReplicantPlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	// DOREPLIFETIME_CONDITION();
}

void AReplicantPlayerCharacter::HandleFire_Implementation()
{
	// AActor* owner = GetOwner();
	// hmm ... even tho we are on server this actor should be client owned??
	// check for owner here

	// TODO: refactor out the spawn parameter logic to its own shared method with spawncosmetic?
	// TODO: have the cosmetic and player projectile share an abstract projectile parent?
	HandleFireMulticast();
	///// HMMMMMMMMMMMM
	//// simulate on the rest of the clients...? server event RPC?
	/// a simple prediction for the other clients -> maybe start position etc. and then simulate locally
	
}

void AReplicantPlayerCharacter::HandleFireMulticast_Implementation()
{
	// FIXME: FOUND THE PROBLEM...DONT NEED THE MULTICAST...
	// only need to spawn on server and it would be auto replicated...
	// if we use multicast then an instance is spawned for every client

	// Multicast is mainly used for FX? ... instead of replicating properties that has to listen all the time
	// COSMETIC ONLY.

	if (GetLocalRole() == ROLE_AutonomousProxy) // Spawn for the server and all simulated proxies
	{
		return;
	}

	SpawnProjectile();
}

void AReplicantPlayerCharacter::StartFire()
{
	if (!bIsFiringWeapon)
	{
		UE_LOG(LogTemp, Warning, TEXT("Firing!"));
		bIsFiringWeapon = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(FiringTimer, this, &AReplicantPlayerCharacter::StopFire, FireRate, false);

		SpawnCosmeticProjectile(); // TODO: THIS IS THE ONLY PROJECTILE TO ADD INERTIA TO ... 
		HandleFire(); // Server
	}
}

void AReplicantPlayerCharacter::StopFire()
{
	bIsFiringWeapon = false;
}

void AReplicantPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AReplicantPlayerCharacter::Move);

		EnhancedInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AReplicantPlayerCharacter::StartFire);
		EnhancedInputComponent->BindAction(BlastDeflectAction, ETriggerEvent::Triggered, this, &AReplicantPlayerCharacter::StartAbility);
	}
}

void AReplicantPlayerCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}

}

void AReplicantPlayerCharacter::StartAbility()
{
	if (!bIsUsingAbility)
	{
		UE_LOG(LogTemp, Warning, TEXT("Ability CHECKED!"));
		bIsUsingAbility = true;
		UWorld* World = GetWorld();
		World->GetTimerManager().SetTimer(AbilityTimer, this, &AReplicantPlayerCharacter::StopAbility, FireRate, false);
		HandleAbility();
	}
}

void AReplicantPlayerCharacter::StopAbility()
{
	bIsUsingAbility = false;
}


void AReplicantPlayerCharacter::HandleAbility_Implementation()
{
	FVector spawnLocation = GetActorLocation() + (GetActorRotation().Vector() * 100.0f) + (GetActorUpVector() * 50.0f);
	FRotator spawnRotation = GetActorRotation();

	FActorSpawnParameters spawnParameters;
	//Instigator is the actor causing this to happen       
	spawnParameters.Instigator = GetInstigator();
	//Owner is the Controller Class that spawned the instigator
	spawnParameters.Owner = this;

	ABlastDeflectAbility* spawnedAbility = GetWorld()->SpawnActor<ABlastDeflectAbility>(AbilityClass, spawnLocation, spawnRotation, spawnParameters);
}
