// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "ReplicantAI.generated.h"

UCLASS()
class REPLICANT_API AReplicantAI : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AReplicantAI();

	UPROPERTY(EditAnywhere, Category = "Pawn")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Components")
	class UStaticMeshComponent* StaticMesh;
	
	// Set in BP with BP_ReplicantAIController
	// Pawn has a built in attribute called AI Controller Class...
	// This is just a forward declaration of our ReplicantAIControllerClass
	class AReplicantAIController* AIController;

	UPROPERTY(VisibleAnywhere, Category = "BehaviorTree")
	class USphereComponent* AgroSphere; 

	UPROPERTY(VisibleAnywhere, Category = "BehaviorTree")
	class USphereComponent* AttackSphere;

	//TODO: TArray<TObjectPtr<AActor>> MoveIgnoreActors; ?? theres a addunique for this array type
	// TArray<AActor*> TargetActors;
	TArray<TObjectPtr<AActor>> TargetActors;

	UFUNCTION()
	void AgroSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AgroSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void AttackSphereBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void AttackSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()                                                                                                                     
	void ConnectReplicantCharacterTarget(AActor* TargetActor, bool SetConnection);

	UFUNCTION()
	void IsReplicantCharacterInAttackRange(AActor* TargetActor, bool InRange);

	UFUNCTION()
	void SetTarget(AActor* TargetActor);

	UFUNCTION()
	AActor* FindClosestActor(TArray<AActor*> Actors);

	UFUNCTION()
	void SetIsInAttackRange(bool Value);

protected:
	virtual void PostInitializeComponents() override;

	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Spawner")
	class UReplicantProjectileSpawnerComp* ProjectileSpawner;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
