#include "Item/ItemSubsystem.h"
#include "Helper.h"

DEFINE_LOG_CATEGORY(ItemSubsystemLog);

void UItemSubsystem::Initialize(FSubsystemCollectionBase& Collection)
{
	Super::Initialize(Collection);

	ItemDataTable = Helper::GetAssetDynamic<UDataTable>(TEXT("/Game/Item/DT_ItemData"));
	EquipDataTable = Helper::GetAssetDynamic<UDataTable>(TEXT("/Game/Item/DT_EquipData"));
	WeaponDataTable = Helper::GetAssetDynamic<UDataTable>(TEXT("/Game/Item/DT_WeaponData"));

	CachingDataTable();
}

void UItemSubsystem::CachingDataTable()
{
	// Item
	if (!ItemDataTable)
	{
		UE_LOG(ItemSubsystemLog, Warning, TEXT("ItemDataTable Is NULL !!"));
		return;
	}
	for (auto& Row : ItemDataTable->GetRowMap())
	{
		FItemData* data = ItemDataTable->FindRow<FItemData>(Row.Key, TEXT("Caching ItemData"), true);

		if (data)
			ItemDataCache.Add(Row.Key, *data);
	}

	// Equip
	if (!EquipDataTable)
	{
		UE_LOG(ItemSubsystemLog, Warning, TEXT("EquipDataTable Is NULL !!"));
		return;
	}
	for (auto& Row : EquipDataTable->GetRowMap())
	{
		FEquipDataTable* RowData = EquipDataTable->FindRow<FEquipDataTable>(Row.Key, TEXT("Caching Equipment Data"), true);
		if (RowData)
		{
			FEquipData data;
			data.InitFromRow(*RowData);
			EquipDataCache.Add(Row.Key, data);
		}
	}

	// Weapon
	if (!WeaponDataTable)
	{
		UE_LOG(ItemSubsystemLog, Warning, TEXT("WeaponDataTable Is NULL !!"));
		return;
	}
	for (auto& Row : WeaponDataTable->GetRowMap())
	{
		FWeaponDataTable* RowData = WeaponDataTable->FindRow<FWeaponDataTable>(Row.Key, TEXT("Caching Weapon Data"), true);
		if (RowData)
		{
			FWeaponData data;
			data.InitFromRow(*RowData);
			WeaponDataCache.Add(Row.Key, data);
		}
	}
}

FWeaponData UItemSubsystem::GetWeaponData(FName ID)
{
	return WeaponDataCache.FindRef(ID);
}
