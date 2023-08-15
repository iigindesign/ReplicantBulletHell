// Fill out your copyright notice in the Description page of Project Settings.


#include "ReplicantProjectileSpawnerComp.h"
#include "ReplicantProjectile.h"

#include "Net/UnrealNetwork.h"
#include "Engine/Engine.h"
#include "Math/Vector.h"
// Sets default values for this component's properties
UReplicantProjectileSpawnerComp::UReplicantProjectileSpawnerComp()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
	bCanSpawnProjectile = false;
	// ...
}

// Called when the game starts
void UReplicantProjectileSpawnerComp::BeginPlay()
{
	Super::BeginPlay();
	if (!bCanSpawnProjectile) 
	{
		return;
	}
	// TODO:
	// putting here for now, but the number of spawners and rotation should be able to be changed dynamically at runtime

	float step = 360.0f / static_cast<float>(SpawnCount);

	for (int i = 0; i < SpawnCount; i++)
	{
		USceneComponent* NewSceneComp = NewObject<USceneComponent>(GetOwner(), USceneComponent::StaticClass());
		NewSceneComp->RegisterComponent();
		NewSceneComp->AttachToComponent(this, FAttachmentTransformRules::KeepWorldTransform);

		FVector position = FVector(SpawnerRadius, 0.0f, 0.0f);
		position = position.RotateAngleAxis(step*i, FVector(0.f, 0.f, 1.f));

		//FString posMessage = FString::Printf(TEXT("POSITION %f %f %f STEP %f"), position.X, position.Y, position.Z, step*i);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Blue, posMessage);


		NewSceneComp->SetRelativeLocation(position);
		NewSceneComp->SetRelativeRotation(position.Rotation());

		//FString rotMessage = FString::Printf(TEXT("Rotation %f %f %f"), position.Rotation().Roll, position.Rotation().Pitch, position.Rotation().Yaw);
		//GEngine->AddOnScreenDebugMessage(-1, 100.f, FColor::Yellow, rotMessage);
	}

	UWorld* World = GetWorld();
	World->GetTimerManager().SetTimer(FiringTimer, this, &UReplicantProjectileSpawnerComp::SpawnProjectile, SpawnRateInSec, true);
	
}


// Called every frame
void UReplicantProjectileSpawnerComp::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

void UReplicantProjectileSpawnerComp::SpawnProjectile()
{
	// Getting all children components
	TArray<USceneComponent*> AllChildren;
	GetChildrenComponents(true, AllChildren);
	// TODO: there should probably be a validation to make sure the child components are empty spawners

	for (int i = 0; i < AllChildren.Num(); ++i)
	{
		FVector spawnLocation = AllChildren[i]->GetComponentLocation();
		FRotator spawnRotation = AllChildren[i]->GetComponentRotation();

		//FString rotMessage = FString::Printf(TEXT("Rotation %f %f %f"), spawnRotation.Roll, spawnRotation.Pitch, spawnRotation.Yaw);
		//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, rotMessage);

		FActorSpawnParameters spawnParameters;

		spawnParameters.Owner = GetOwner();

		AReplicantProjectile* spawnedProjectile = GetWorld()->SpawnActor<AReplicantProjectile>(ProjectileClass, spawnLocation, spawnRotation, spawnParameters);

	}
}

