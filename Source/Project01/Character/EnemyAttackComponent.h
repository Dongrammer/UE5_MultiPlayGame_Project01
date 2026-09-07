#pragma once

#include "CoreMinimal.h"
#include "Character/AttackComponent.h"
#include "EnemyAttackComponent.generated.h"

UCLASS()
class PROJECT01_API UEnemyAttackComponent : public UAttackComponent
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	/* ==================== Targeting ==================== */
public:
	void SelectAttackTarget() override;

	/* ==================== Attack ==================== */
protected:
	void DoAttack() override;

	/* ==================== Notify ==================== */
public:
	void FireProjectileNotify() override;
	void EndAttackNotify() override;
};
