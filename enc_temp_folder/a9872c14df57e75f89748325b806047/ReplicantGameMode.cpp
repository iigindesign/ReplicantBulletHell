// Copyright Epic Games, Inc. All Rights Reserved.

#include "ReplicantGameMode.h"
#include "ReplicantCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Engine/World.h"
#include "ReplicantAIController.h"
#include "ReplicantAI.h"

AReplicantGameMode::AReplicantGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Replicant/ReplicantCharacter_BP"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}

void AReplicantGameMode::BeginPlay()
{
	if (HasAuthority())
	{
		ServerCreateBots();
	}
}

// Lyra spawns using a gamestate component, do we even need to check for auth if spawning from gamemode?
void AReplicantGameMode::ServerCreateBots()
{
	if (BotControllerClass == nullptr)
	{
		return;
	}
	SpawnOneBot();
}

void AReplicantGameMode::SpawnOneBot()
{
	UE_LOG(LogTemp, Warning, TEXT("Spawn bottttt"));

	FActorSpawnParameters SpawnInfo;
	SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	AAIController* NewController = GetWorld()->SpawnActor<AAIController>(BotControllerClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
	if (NewController != nullptr)
	{
		RestartPlayer(NewController);
	}
}

UClass* AReplicantGameMode::GetDefaultPawnClassForController_Implementation(AController* InController)
{
	//if (const ULyraPawnData* PawnData = GetPawnDataForController(InController))
	//{
	//	if (PawnData->PawnClass)
	//	{
	//		return PawnData->PawnClass;
	//	}
	//}
	AReplicantAIController* AIController = Cast<AReplicantAIController>(InController);
	if (AIController)
	{
		if (BotClass)
		{
			return BotClass;
		}
	}

	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
