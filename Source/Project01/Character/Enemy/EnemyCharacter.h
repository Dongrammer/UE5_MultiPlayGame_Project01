#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"

#include "EnemyCharacter.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(EnemyLog, Log, All);

UENUM(BlueprintType)
enum class EEnemyChaseState : uint8
{
	E_Waiting UMETA(DisplayName = "Waiting"),
	E_Chase UMETA(DisplayName = "Chase")
};

class UWidgetComponent;

UCLASS()
class PROJECT01_API AEnemyCharacter : public ABaseCharacter
{
	GENERATED_BODY()
	
public:
	AEnemyCharacter();
	virtual void Tick(float DeltaSeconds) override;

protected:
	virtual void BeginPlay() override;

	/* ==================== World ==================== */
protected:
	void StartRunning() override;

	/* ==================== Attack ==================== */

public:
	void SetFrontBack(bool front) override;
	void ChaseHeroTick(float Dt);

	/* ==================== Damage ==================== */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TObjectPtr<UWidgetComponent> WB_HPBar;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential")
	TSubclassOf<UUserWidget> WB_HPBarClass;

public:
	virtual void OnRep_HealthPoint() override;
};
