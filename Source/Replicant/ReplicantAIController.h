// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#include "AIController.h"
#include "ReplicantAIController.generated.h"

/**
 * 
 */
UCLASS()
class REPLICANT_API AReplicantAIController : public AAIController
{
	GENERATED_BODY()

	public:
		AReplicantAIController();

		virtual void OnPossess(APawn* InPawn) override;

		UBlackboardComponent* GetBlackboard();

		// TODO: I really dont like hard coded AI patrol points. Replace with random walk later.

		// MakeEditWidget creates a level widget; this populates with relative location to this character; Get world location by adding actor's location.
		UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
		FVector PatrolPoint1;

		UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
		FVector PatrolPoint2;

		UPROPERTY(EditAnywhere, Category = "BehaviorTree", Meta = (MakeEditWidget = true))
		FVector PatrolPoint3;

	protected:
		UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBlackboardComponent* BlackboardComponent;

		UPROPERTY(BlueprintReadWrite, Category = "Behavior")
		class UBehaviorTreeComponent* BehaviorTreeComponent;
};
