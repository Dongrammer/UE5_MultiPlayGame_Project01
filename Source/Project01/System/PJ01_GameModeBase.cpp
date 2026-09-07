#include "PJ01_GameModeBase.h"
#include "Helper.h"
#include "PJ01_GameStateBase.h"

DEFINE_LOG_CATEGORY(GameModeLog);

APJ01_GameModeBase::APJ01_GameModeBase()
{
	GameStateClass = APJ01_GameStateBase::StaticClass();
}

void APJ01_GameModeBase::StartPlay()
{
	Super::StartPlay();

}