#include "Character/Enemy/EnemyCatalogDataAsset.h"

TSubclassOf<AEnemyCharacter> UEnemyCatalogDataAsset::GetEnemyClass(EEnemyType type, bool bNamed) const
{
	if (const FEnemyNamedType* Found = EnemyClass.Find(type))
	{
		return bNamed ? Found->NamedClass.LoadSynchronous() : Found->NormalClass.LoadSynchronous();
	}

	return nullptr;
}
