#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"

#include "WorldData.h"

#include "WorldManager.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(WorldManagerLog, Log, All);
DECLARE_MULTICAST_DELEGATE_OneParam(FWorldSpeedChanged, float);
DECLARE_MULTICAST_DELEGATE_OneParam(FWorldTimeChaged, int);

class UEnemySpawnComponent;
class AEnemySpawnArea;
class UEnemyCatalogDataAsset;
class ABaseCharacter;

UCLASS()
class PROJECT01_API AWorldManager : public AActor
{
	GENERATED_BODY()
	
public:	
	AWorldManager();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	void StartWorld();
	void StopWorld();

	/* ==================== StageStatus ====================*/
private:
	UPROPERTY(ReplicatedUsing = OnRep_WorldSpeed, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float WorldSpeed = 0.0f;
	UPROPERTY(ReplicatedUsing = OnRep_ElapsedTime, EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int ElapsedTime = 0;
	FTimerHandle THandle_ElapsedTime;

public:
	FWorldSpeedChanged DWorldSpeedChanged;
	FWorldTimeChaged DWorldTimeChanged;

public:
	FORCEINLINE float GetWorldSpeed() const { return WorldSpeed; }

	UFUNCTION(BlueprintCallable)
	void SetWorldSpeed(float NewSpeed);
	
	void ApplyWorldSpeed();

	UFUNCTION()
	void TickElapsedTime();

	/* ==================== EnemySpawn ====================*/
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEnemyCatalogDataAsset> EnemyCatalog;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEnemySpawnComponent> EnemySpawnComponent;

	// SpawnArea
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AEnemySpawnArea> SpawnAreaFront;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AEnemySpawnArea> SpawnAreaBack;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AEnemySpawnArea> SpawnAreaClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	FVector FrontSpawnLocation = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	FVector BackSpawnLocation = FVector::ZeroVector;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	FVector SpawnAreaSize = FVector::ZeroVector;

public:
	void StartEnemySpawn(FStageEnemyStatusTable* data);
	void EnemySpawn(EEnemyType type, bool bnamed);

	/* ==================== Hero ====================*/
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<TObjectPtr<ABaseCharacter>> Heros;

public:
	void AddHero(ABaseCharacter* hero);
	TArray<TObjectPtr<ABaseCharacter>> GetHeros();

	/* ==================== Server ====================*/
private:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(Server, Reliable)
	void Server_SetWorldSpeed(float NewSpeed);
	UFUNCTION(Server, Reliable)
	void Server_ElapsedTimeStart();

	/* ==================== OnRep ====================*/
	UFUNCTION()
	void OnRep_WorldSpeed();
	UFUNCTION()
	void OnRep_ElapsedTime();
};
