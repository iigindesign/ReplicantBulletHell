// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "ReplicantAI.h"

AReplicantAIController::AReplicantAIController()
{
	if (!HasAuthority())
	{
		return;
	}
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("ReplicantControllerBlackboard"));
	check(BlackboardComponent);

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("ReplicantBehaviorTree"));
	check(BehaviorTreeComponent);
}

void AReplicantAIController::OnPossess(APawn* InPawn)
{
	if (!HasAuthority())
	{
		return;
	}
	Super::OnPossess(InPawn);
	if (InPawn == nullptr)
	{
		return;
	}
	AReplicantAI* AI = Cast<AReplicantAI>(InPawn);
	if (AI)
	{
		if (AI->BehaviorTree)
		{
			BlackboardComponent->InitializeBlackboard(*(AI->BehaviorTree->BlackboardAsset));
			BehaviorTreeComponent->StartTree(*(AI->BehaviorTree));

			//TODO: behavior tree is stuck...at the first wait
			FVector Location = AI->GetActorLocation();
			GetBlackboard()->SetValueAsVector(TEXT("PatrolPoint1"), PatrolPoint1 + Location);
			GetBlackboard()->SetValueAsVector(TEXT("PatrolPoint2"), PatrolPoint2 + Location);
			GetBlackboard()->SetValueAsVector(TEXT("PatrolPoint3"), PatrolPoint3 + Location);
		}
	}
}

UBlackboardComponent* AReplicantAIController::GetBlackboard()
{
	return BlackboardComponent;
}
