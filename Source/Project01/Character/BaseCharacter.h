#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"

#include "BaseCharacter.generated.h"

class AWorldManager;
enum class EGamePhase : uint8;
class UAttackComponent;

UCLASS()
class PROJECT01_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	ABaseCharacter();

protected:
	virtual void BeginPlay() override;

	/* ==================== Body ==================== */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> TopRootComponent; // Don't touch

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UCapsuleComponent> BodyCollision;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Team")
	uint8 TeamID = 0; // 0 = Player, 1 = Enemy

public:	
	virtual void Tick(float DeltaTime) override;
	FORCEINLINE uint8 GetTeamID() const { return TeamID; }
	void SetCharacterMovement();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* ==================== World ==================== */
protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadOnly, Category = "World", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWorldManager> WorldManager;
	
	UFUNCTION()
	virtual void WorldManagerSetting(AWorldManager* wm);

	UFUNCTION(Server, Reliable)
	void Server_StartRunning();

	virtual void StartRunning();
	UFUNCTION()
	virtual void GamePhaseChanged(EGamePhase phase);
	UFUNCTION()
	virtual void WorldSpeedChanged(float NewSpeed);

	/* ==================== Movement ==================== */
protected:
	UPROPERTY(Replicated, VisibleAnywhere, BlueprintReadWrite)
	bool bShouldMove = false;
	UPROPERTY(Replicated, EditAnywhere, BlueprintReadWrite, Category = "Speed")
	float Speed = 0.0; 
	UPROPERTY(ReplicatedUsing = OnRep_InFront, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool bInFront = true;

	// Client
public:
	UFUNCTION(BlueprintCallable)
	virtual void SetFrontBack(bool front);

	void SetShouldMove(bool m) { bShouldMove = m; }
	FORCEINLINE bool GetShouldMove() const { return bShouldMove; }
	FORCEINLINE bool GetFrontBack() const { return bInFront; }
	FORCEINLINE float GetSpeed() const { return Speed; };
	// Server
public:
	UFUNCTION(Server, Reliable)
	void Server_SetFrontBack(bool front);
	UFUNCTION()
	virtual void OnRep_InFront();

	/* ==================== Attack ==================== */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Attack")
	TObjectPtr<UAttackComponent> AttackComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool bShouldAttack = false;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float RetargetInterval = 1.0f;

public:
	virtual void RotateToTarget();

public:
	FORCEINLINE bool GetShouldAttack() const { return bShouldAttack; }
	TObjectPtr<UAttackComponent> GetAttackComponent() const { return AttackComponent; }
	float GetAttackSpeed() const;

	/* ==================== Select Target ==================== */
public:
	UFUNCTION(BlueprintCallable)
	void SelectTarget();

	// Server
public:
	UFUNCTION(Server, Reliable)
	void Server_SelectTarget();

	/* ==================== Damage ==================== */
protected:
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite)
	float MaxHealthPoint = 100.f;
	UPROPERTY(ReplicatedUsing = OnRep_HealthPoint, EditDefaultsOnly, BlueprintReadWrite)
	float HealthPoint = 100.f;

public:
	UFUNCTION(Server, Reliable)
	void Server_TakeAttackDamage(const float damage);
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Die();
	UFUNCTION()
	virtual void OnRep_HealthPoint();
};
