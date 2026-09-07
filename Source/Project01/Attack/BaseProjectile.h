#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BaseProjectile.generated.h"

class UProjectileMovementComponent;
class USphereComponent;

USTRUCT(BlueprintType)
struct FProjectileData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float Damage = 40.f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	float KnockbackDamage = 100.f;
};

UCLASS()
class PROJECT01_API ABaseProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	ABaseProjectile();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<USphereComponent> CollisionComponent;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UStaticMeshComponent> SMeshComponent;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UProjectileMovementComponent> MovementComponent;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	FProjectileData ProjectileData;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float LifeSeconds = 5.0f;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UParticleSystem> ImpactEffect;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<USoundBase> ImpactSound;

public:
	void InitVelocity(const FVector& ShootDir);

protected:
	UFUNCTION()
	void OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(NetMulticast, Unreliable)
	void Multicast_PlayImpactFX(FVector_NetQuantize Location);
	
	void ApplyAttack(AActor* OtherActor, const FHitResult& Hit);
};
