#pragma once

#include "CoreMinimal.h"
#include "ItemData.generated.h"

UENUM(BlueprintType)
enum class EItemType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Weapon UMETA(DisplayName = "Weapon"),
	E_Equip UMETA(DIsplayName = "Equip"),
	E_Other UMETA(DisplayName = "Other"),
	E_Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EEquipSlot : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Head UMETA(DisplayName = "Head"),
	E_Armor UMETA(DusplayName = "Armor"),
	E_Shoes UMETA(DusplayName = "Armor"),
	E_Weapon UMETA(DisplayName = "Weapon"),
	E_Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class ERarity : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Common UMETA(DisplayName = "Common"),
	E_Rare UMETA(DusplayName = "Rare"),
	E_Unique UMETA(DusplayName = "Unique"),
	E_Legendary UMETA(DisplayName = "Legendary"),
	E_Max UMETA(Hidden)
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Dart UMETA(DisplayName = "Dart"),
	E_Sword UMETA(DisplayName = "Sword"),
	E_Bow UMETA(DisplayName = "Bow"),
	E_Staff UMETA(DisplayName = "Staff"),
	E_Max UMETA(Hidden)
};

/* ==================== DataTable ====================*/

USTRUCT(BlueprintType)
struct FItemDataTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemType ItemType = EItemType::E_Other;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	TSoftObjectPtr<UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BasePrice = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ERarity Rarity = ERarity::E_Common;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int MaxStack = 1;
};

USTRUCT(BlueprintType)
struct FEquipDataTable : public FItemDataTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	EEquipSlot EquipSlot = EEquipSlot::E_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	TSoftObjectPtr<USkeletalMesh> SkeletalMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	int MaxUpgrade = 10;
};

class ABaseProjectile;

USTRUCT(BlueprintType)
struct FWeaponDataTable : public FEquipDataTable
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::E_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackRate = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float AttackSpeed = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ProjectileSpeed = 1500.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float Damage = 10.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float KnockbackDamage = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSoftClassPtr<ABaseProjectile> ProjectileClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSoftObjectPtr<UParticleSystem> FireEffect;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSoftObjectPtr<USoundBase> FireSound;
};

/* ==================== DataStruct ====================*/

USTRUCT(BlueprintType)
struct FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	EItemType ItemType = EItemType::E_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Item")
	FName ItemID;

	void InitFromRow(const FItemDataTable& Row)
	{
		ItemType = Row.ItemType;
		ItemID = Row.ItemID;
	}
};

USTRUCT(BlueprintType)
struct FEquipData : public FItemData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Equip")
	TObjectPtr<USkeletalMesh> SkeletalMesh;

	void InitFromRow(const FEquipDataTable& Row)
	{
		ItemType = Row.ItemType;
		ItemID = Row.ItemID;
		SkeletalMesh = Row.SkeletalMesh.LoadSynchronous();
	}
};

USTRUCT(BlueprintType)
struct FWeaponData : public FEquipData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	EWeaponType WeaponType = EWeaponType::E_None;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TSubclassOf<ABaseProjectile> ProjectileClass = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<UParticleSystem> FireEffect = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	TObjectPtr<USoundBase> FireSound = nullptr;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Weapon")
	float ProjectileSpeed = 300.f;
	
	void InitFromRow(const FWeaponDataTable& Row)
	{
		ItemType = Row.ItemType;
		ItemID = Row.ItemID;
		SkeletalMesh = Row.SkeletalMesh.LoadSynchronous();
		WeaponType = Row.WeaponType;
		ProjectileClass = Row.ProjectileClass.LoadSynchronous();
		FireEffect = Row.FireEffect.LoadSynchronous();
		FireSound = Row.FireSound.LoadSynchronous();
		ProjectileSpeed = Row.ProjectileSpeed;
	}
};