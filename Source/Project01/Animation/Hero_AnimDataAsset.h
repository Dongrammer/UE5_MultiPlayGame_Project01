#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "../Item/ItemData.h"
#include "Hero_AnimDataAsset.generated.h"

UCLASS()
class PROJECT01_API UHero_AnimDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	TMap<EWeaponType, TObjectPtr<UAnimMontage>> WeaponAnims;
};
