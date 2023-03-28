// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ReplicantGameMode.generated.h"

class AReplicantAIController;
class AReplicantAI;

UCLASS()
class AReplicantGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AReplicantGameMode();

	virtual void BeginPlay() override;
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

protected:
	virtual void ServerCreateBots();
	virtual void SpawnOneBot();

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<AReplicantAIController> BotControllerClass;

	UPROPERTY(EditDefaultsOnly, Category = Gameplay)
	TSubclassOf<AReplicantAI> BotClass;
};



