#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
//#include "EquipDataAsset.h"
//#include "ItemDataAsset.h"
//#include "WeaponDataAsset.h"
#include "AllItemDataAsset.generated.h"

UCLASS()
class PROJECT01_API UAllItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential")
	TObjectPtr<UDataTable> ItemDataTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential")
	TObjectPtr<UDataTable> EquipDataAsset;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential")
	TObjectPtr<UDataTable> WeaponDataAsset;
};
