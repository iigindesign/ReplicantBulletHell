#pragma once

#include "CoreMinimal.h"
#include "ReplicantProjectile.h"
#include "PlayerProjectile.generated.h"


UCLASS()
class REPLICANT_API APlayerProjectile : public AReplicantProjectile
{
    GENERATED_BODY()

public:
    APlayerProjectile();

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = Components)
    class USphereComponent* SphereComponent;

protected:
    virtual void OnProjectileImpact(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;

    UPROPERTY(EditAnywhere, Category = Cosmetic)
    class UStaticMesh* ProjectileMesh;


public:
};
