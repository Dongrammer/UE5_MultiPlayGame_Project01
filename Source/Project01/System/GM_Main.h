#pragma once

#include "CoreMinimal.h"
#include "PJ01_GameModeBase.h"
#include "GM_Main.generated.h"

UCLASS()
class PROJECT01_API AGM_Main : public APJ01_GameModeBase
{
	GENERATED_BODY()
	
protected:
	AGM_Main();

	void StartPlay() override;

};
