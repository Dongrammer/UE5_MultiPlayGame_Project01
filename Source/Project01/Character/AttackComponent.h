#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Item/ItemData.h"
#include "AttackComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(AttackCompLog, Log, All);

DECLARE_MULTICAST_DELEGATE(FDoAttack);

class ABaseCharacter;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT01_API UAttackComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UAttackComponent();
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<ABaseCharacter> OwnerCharacter;

	/* ==================== Targeting ==================== */
protected:
	UPROPERTY(ReplicatedUsing = OnRep_AttackTarget, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ABaseCharacter> AttackTarget;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float AttackRange = 1200.0f;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Attack")
	float HalfFOV = 80.0f; // Àü¹æ *2µµ

public:
	virtual void SelectAttackTarget();
	UFUNCTION()
	void OnRep_AttackTarget();

	// ForceSelectting
	UFUNCTION(BlueprintCallable)
	void SetAttackTarget(ABaseCharacter* target);
	UFUNCTION(Server, Reliable)
	void Server_SetAttackTarget(ABaseCharacter* target);

public:
	FORCEINLINE TObjectPtr<ABaseCharacter> GetAttackTarget() const { return AttackTarget; }

	/* ==================== Attack ==================== */
protected:
	bool bAutoAttack = false;
	bool bCanNextAttack = true;

	UFUNCTION(BlueprintCallable)
	virtual void DoAttack();
	UFUNCTION()
	void SetNextAttack();
	FTimerHandle AttackTimer;

public:
	virtual void StartAutoAttack();
	void StopAutoAttack();

	FDoAttack DDoAttack;

	/* ==================== Attack Rate ==================== */
protected:
	UPROPERTY(ReplicatedUsing = OnRep_AttackRate, EditDefaultsOnly, BlueprintReadWrite, Category = "Attack", meta = (AllowPrivateAccess = "true"))
	float AttackRate = 1.0f;

	float AttackSpeed = 1.0f;
	float AttackCooldown = 1.0f;

public:
	UFUNCTION(BlueprintCallable)
	void SetAttackRate(float rate);
	UFUNCTION(Server, Reliable)
	void Server_SetAttackRate(float rate);
	UFUNCTION()
	void OnRep_AttackRate();
	FORCEINLINE float GetAttackRate() const { return AttackRate; }


	/* ==================== Projectile ==================== */
	

	/* ==================== Notify ==================== */
public:
	UFUNCTION()
	virtual void FireProjectileNotify();
	UFUNCTION()
	virtual void EndAttackNotify();

};
