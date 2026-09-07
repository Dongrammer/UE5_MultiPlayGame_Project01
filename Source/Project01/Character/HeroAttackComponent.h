#pragma once

#include "CoreMinimal.h"
#include "Character/AttackComponent.h"
#include "HeroAttackComponent.generated.h"

class UHero_AnimDataAsset;
// Weapon
class UItemSubsystem;

UCLASS()
class PROJECT01_API UHeroAttackComponent : public UAttackComponent
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	virtual void BeginPlay() override;

	/* ==================== Targeting ==================== */
public:
	void SelectAttackTarget() override;

	/* ==================== Attack ==================== */
protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Attack|Essential", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UHero_AnimDataAsset> HeroAnim;

	void DoAttack() override;

public:
	UFUNCTION(Server, Reliable)
	void Server_DoAttack();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayAttackMontage();

	/* ==================== Item Data ==================== */
protected:
	TObjectPtr<UItemSubsystem> ItemSubsystem;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FName CurrentWeaponID;
	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon", meta = (AllowPrivateAccess = "true"))
	FWeaponData CurrentWeaponData;

public:
	UFUNCTION(Server, Reliable)
	void Server_SetCurrentWeapon(FName WeaponID);

	/* ==================== Notify ==================== */
public:
	void FireProjectileNotify() override;
	void EndAttackNotify() override;
};
