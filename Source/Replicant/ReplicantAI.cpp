// Fill out your copyright notice in the Description page of Project Settings.
#include "ReplicantAI.h"

#include "ReplicantProjectileSpawnerComp.h"
#include "ReplicantCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "ReplicantAIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/SphereComponent.h"
#include "Net/UnrealNetwork.h"
#include "DrawDebugHelpers.h"

// Sets default values
AReplicantAI::AReplicantAI()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	ProjectileSpawner = CreateDefaultSubobject<UReplicantProjectileSpawnerComp>(TEXT("ProjectileSpawner"));
	ProjectileSpawner->SetupAttachment(RootComponent);
	ProjectileSpawner->SetIsReplicated(true);

	AggroSphere = CreateDefaultSubobject<USphereComponent>(TEXT("AggroSphere"));
	AggroSphere->SetupAttachment(RootComponent);
}

void AReplicantAI::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	// TODO: THIS IS VERY JANK. NEED TO FIND BETTER SOLUTION TO ONLY CONSTRUCT THIS COMPONENT ON SERVER
	if (!HasAuthority())
	{
		ProjectileSpawner->DestroyComponent();
	}
}

// Called when the game starts or when spawned
void AReplicantAI::BeginPlay()
{
	Super::BeginPlay();
	
	if (!HasAuthority())
	{
		return;
	}

	//TSubclassOf<AActor> ReplicantCharacterClass = AReplicantCharacter::StaticClass();
	//TArray<AActor*> ReplicantCharacters;
	//UGameplayStatics::GetAllActorsOfClass(this, ReplicantCharacterClass, ReplicantCharacters);
	//if (ReplicantCharacters.Num() > 0)
	//{
	//	UNavigationPath* NavPath = UNavigationSystemV1::FindPathToActorSynchronously(this, GetActorLocation(), ReplicantCharacters[0]);
	//	if (NavPath->PathPoints.Num() > 0)
	//	{
	//		for (auto pt : NavPath->PathPoints)
	//		{
	//			DrawDebugSphere(GetWorld(), pt, 20.f, 12, FColor::Red, true);
	//		}
	//	}
	//}


	//TODO: look into unreal delegates!
	AggroSphere->OnComponentBeginOverlap.AddDynamic(this, &AReplicantAI::AggroSphereBeginOverlap);
	AggroSphere->OnComponentEndOverlap.AddDynamic(this, &AReplicantAI::AggroSphereEndOverlap);
}

// Called every frame
void AReplicantAI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	DrawDebugSphere(GetWorld(), GetActorLocation(), AggroSphere->GetScaledSphereRadius(), 12, FColor::Red, false, 0.1);
	if (TargetActors.Num() > 1) {
		SetTarget(FindClosestActor(TargetActors));
	}
}

void AReplicantAI::AggroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	ConnectReplicantCharacterTarget(OtherActor, true);
}

void AReplicantAI::AggroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	ConnectReplicantCharacterTarget(OtherActor, false);
}

void AReplicantAI::AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	IsReplicantCharacterInAttackRange(OtherActor, true);
}

void AReplicantAI::AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	IsReplicantCharacterInAttackRange(OtherActor, false);
}

void AReplicantAI::ConnectReplicantCharacterTarget(AActor* TargetActor, bool SetConnection)
{
	if (TargetActor)
	{
		AReplicantCharacter* ReplicantCharacter = Cast<AReplicantCharacter>(TargetActor);
		if (ReplicantCharacter)
		{
			if (SetConnection) {
				TargetActors.AddUnique(TargetActor);

				FString name = TargetActor->GetName();
				// TODO: why dereference here?? for UE_LOG
				if (TargetActors.Num() == 1)
				{
					SetTarget(TargetActor);
				}
			}
			else
			{
				TargetActors.RemoveSingleSwap(TargetActor);
				if (TargetActors.Num() == 0)
				{
					SetTarget(nullptr);
				}
			}
		}
	}
}

void AReplicantAI::IsReplicantCharacterInAttackRange(AActor* TargetActor, bool InRange)
{
	if (TargetActor)
	{
		AReplicantCharacter* ReplicantCharacter = Cast<AReplicantCharacter>(TargetActor);
		if (ReplicantCharacter)
		{
			SetIsInAttackRange(InRange);
		}
	}
}

void AReplicantAI::SetTarget(AActor* TargetActor)
{
	if (AIController == nullptr)
	{
		AIController = Cast<AReplicantAIController>(GetController());
	}
	AIController->GetBlackboard()->SetValueAsObject(TEXT("TargetActor"), TargetActor);
}

AActor* AReplicantAI::FindClosestActor(TArray<AActor*> Actors)
{
	AActor* ClosestActor = nullptr;
	float ClosestDistance = FLT_MAX;
	for (auto& Actor : Actors)
	{
		float Distance = GetDistanceTo(Actor);
		if (ClosestDistance > Distance)
		{
			ClosestDistance = Distance;
			ClosestActor = Actor;
		}
	}
	return ClosestActor;
}

void AReplicantAI::SetIsInAttackRange(bool Value)
{
	if (AIController == nullptr)
	{
		AIController = Cast<AReplicantAIController>(GetController());
	}
	AIController->GetBlackboard()->SetValueAsBool(TEXT("InAttackRange"), Value);
}

