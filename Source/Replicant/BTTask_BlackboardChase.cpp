// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BlackboardChase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "ReplicantAIController.h"
#include "ReplicantAI.h"
#include "GameFramework/Actor.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"

UBTTask_BlackboardChase::UBTTask_BlackboardChase(const FObjectInitializer& ObjectInitializer)
{
	bCreateNodeInstance = true;
	NodeName = "Chase Target Actor";
}

EBTNodeResult::Type UBTTask_BlackboardChase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	const UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();
	AAIController* AIController = OwnerComp.GetAIOwner();
	
	if (!AIController || !Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	//TODO: Revisit to understand cast turning the input into pointer 

	AActor* Target = Cast<AActor>(Blackboard->GetValueAsObject(TEXT("TargetActor")));
	FVector const TargetLocation = Target->GetActorLocation();

	UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIController, TargetLocation);
	return EBTNodeResult::Succeeded;
}
