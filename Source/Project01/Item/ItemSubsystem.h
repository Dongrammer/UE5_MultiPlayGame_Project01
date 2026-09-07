#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ItemData.h"
#include "ItemSubsystem.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(ItemSubsystemLog, Log, All);

UCLASS()
class PROJECT01_API UItemSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	void Initialize(FSubsystemCollectionBase& Collection) override;

	// Data Tables
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> ItemDataTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> EquipDataTable;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> WeaponDataTable;

	// Caching DataTable
private:
	void CachingDataTable();

	UPROPERTY(VisibleAnywhere, Category = "CachedItems", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FItemData> ItemDataCache;
	UPROPERTY(VisibleAnywhere, Category = "CachedItems", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FEquipData> EquipDataCache;
	UPROPERTY(VisibleAnywhere, Category = "CachedItems", meta = (AllowPrivateAccess = "true"))
	TMap<FName, FWeaponData> WeaponDataCache;

public:
	FWeaponData GetWeaponData(FName ID);
};
