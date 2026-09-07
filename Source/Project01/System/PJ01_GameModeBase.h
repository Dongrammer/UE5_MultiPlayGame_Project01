#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"

DECLARE_LOG_CATEGORY_EXTERN(GameModeLog, Log, All);

#include "PJ01_GameModeBase.generated.h"


UCLASS()
class PROJECT01_API APJ01_GameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	APJ01_GameModeBase();

	virtual void StartPlay() override;


};
