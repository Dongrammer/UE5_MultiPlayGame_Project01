#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../../Item/ItemData.h"
#include "EquipComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(EquipCompLog, Log, All);

DECLARE_MULTICAST_DELEGATE_OneParam(FWeaponChanged, FName);

class AHero;
class ABaseWeapon;
class UItemSubsystem;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT01_API UEquipComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEquipComponent();

protected:
	virtual void BeginPlay() override;
	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	TObjectPtr<AHero> Owner;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	/* ==================== Item Data ==================== */
private:
	TObjectPtr<UItemSubsystem> ItemSubsystem;

	/* ==================== Weapon ==================== */
private:
	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	FName CurrentWeaponID;

public:
	FWeaponChanged DWeaponChanged;

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(FName WeaponID);
	UFUNCTION(Server, Reliable)
	void Server_EquipWeapon(FName WeaponID);
	UFUNCTION()
	void OnRep_CurrentWeapon();
};
