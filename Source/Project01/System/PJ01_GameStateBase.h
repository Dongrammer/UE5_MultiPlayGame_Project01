#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "World/WorldData.h"

DECLARE_LOG_CATEGORY_EXTERN(GameStateLog, Log, All);

#include "PJ01_GameStateBase.generated.h"

class AWorldManager;

DECLARE_MULTICAST_DELEGATE_OneParam(FOnWorldManagerReady, AWorldManager*);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnGamePhaseChanged, EGamePhase);


UCLASS()
class PROJECT01_API APJ01_GameStateBase : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* ==================== WorldManager ====================*/
private:
	UPROPERTY(ReplicatedUsing = OnRep_WorldManager, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWorldManager> WorldManager;

public:
	FOnWorldManagerReady DOnWorldManagerReady;

	virtual void SetWorldManager(AWorldManager* manager);
	UFUNCTION()
	void OnRep_WorldManager();

	virtual TObjectPtr<AWorldManager> GetWorldManager() { return WorldManager; }


	/* ==================== Enemy ====================*/
private:
	FStageEnemyStatusTable* EnemyTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int RemainingEnemies = 0;

	void SpawnEnemy();

	FTimerHandle EnemySpawnHandle;
	/* ==================== GamePhase ====================*/
private:
	UPROPERTY(ReplicatedUsing = OnRep_GamePhase, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	EGamePhase GamePhase = EGamePhase::E_Loading;

public:
	FOnGamePhaseChanged DOnGamePhaseChanged;

	void SetGamePhase(EGamePhase NewPhase);

	UFUNCTION()
	void OnRep_GamePhase();

	FORCEINLINE EGamePhase GetGamePhase() const { return GamePhase; }
public:
	void StartStage(FStageEnemyStatusTable* data);
	void PauseStage();

	/* ==================== MPC ====================*/
private:
	UPROPERTY(VisibleAnywhere, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMaterialParameterCollection> MPCAsset;
	UPROPERTY(ReplicatedUsing = OnRep_MPCValues, VisibleAnywhere, meta = (AllowPrivateAccess = "true"))
	float MPCWorldSpeed = 0.f;

public:
	UFUNCTION()
	void WorldSpeedChanged(float speed);
	UFUNCTION()
	void OnRep_MPCValues();
};
