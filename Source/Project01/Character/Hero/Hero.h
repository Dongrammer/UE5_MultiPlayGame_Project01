#pragma once

#include "CoreMinimal.h"
#include "Character/BaseCharacter.h"
#include "../../Item/AllItemDataAsset.h"

DECLARE_LOG_CATEGORY_EXTERN(HeroLog, Log, All);

#include "Hero.generated.h"

// Widget
class UMainHUD;
// Camera
class USpringArmComponent;
class UCameraComponent;
// Input
class UEnhancedInputComponent;
class UInputMappingContext;
class UInputDataAsset;
struct FInputActionValue;
// Attack
class UHeroAttackComponent;
// Equip
class UEquipComponent;
enum class EWeaponType : uint8;

UCLASS()
class PROJECT01_API AHero : public ABaseCharacter
{
	GENERATED_BODY()

public:
	AHero();

	void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	/* ==================== Loading ==================== */
public:
	FTimerHandle THPlayerReady;

	void CheckPlayerReady();

	/* ==================== Widget ==================== */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainHUD> MainHUD;


	/* ==================== World ==================== */
protected:
	FDelegateHandle TimeChangedHandle;

	virtual void WorldManagerSetting(AWorldManager* wm) override;

public:
	UFUNCTION()
	void OnWorldTimeChanged(int time);

	/* ==================== Camera ==================== */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USpringArmComponent> CameraArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCameraComponent> Camera;

	/* ==================== Input ==================== */
protected:
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;
	void MappingInputAsset(UEnhancedInputComponent* Comp);

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Essential", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputMappingContext> KeyMappingContext;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input|Essential", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputDataAsset> InputAsset;

public:
	void Move(const FInputActionValue& Value);
	void DoJump();
	void DoTurn();
	// Server
public:
	void OnRep_InFront() override;

	/* ==================== World ==================== */
protected:
	void StartRunning() override;

	/* ==================== Attack ==================== */
protected:

	/* ==================== Equip ==================== */
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Equip", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UEquipComponent> EquipComponent;

public:
	UFUNCTION()
	void WeaponChanged(FName WeaponID);
};
