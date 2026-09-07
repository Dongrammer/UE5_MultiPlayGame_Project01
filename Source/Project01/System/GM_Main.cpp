#include "System/GM_Main.h"
#include "Helper.h"
#include "PC_Main.h"

AGM_Main::AGM_Main()
{
	TSubclassOf<APawn> PlayerPawnClass = Helper::GetClass<APawn>(TEXT("/Game/Character/BP_MainMenuCharacter"));

	if (PlayerPawnClass != NULL)
		DefaultPawnClass = PlayerPawnClass;

	TSubclassOf<APC_Main> PlayerContClass = APC_Main::StaticClass();

	if (PlayerContClass != NULL)
		PlayerControllerClass = PlayerContClass;
}

void AGM_Main::StartPlay()
{
	Super::StartPlay();
}
