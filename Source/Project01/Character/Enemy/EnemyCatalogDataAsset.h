#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnemyCatalogDataAsset.generated.h"

class AEnemyCharacter;
enum class EEnemyType : uint8;

USTRUCT(BlueprintType)
struct FEnemyNamedType
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AEnemyCharacter> NormalClass;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSoftClassPtr<AEnemyCharacter> NamedClass;

};

UCLASS()
class PROJECT01_API UEnemyCatalogDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TMap<EEnemyType, FEnemyNamedType> EnemyClass;

	TSubclassOf<AEnemyCharacter> GetEnemyClass(EEnemyType type, bool bNamed) const;
};
