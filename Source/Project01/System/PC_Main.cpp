#include "System/PC_Main.h"
#include "Helper.h"
#include "Widget/MainMenuHUD.h"

DEFINE_LOG_CATEGORY(PCLog);

APC_Main::APC_Main()
{
	MainMenuHUDClass = Helper::GetClass<UMainMenuHUD>(TEXT("/Game/Widget/BP_MainMenuHUD"));
}

void APC_Main::BeginPlay()
{
	Super::BeginPlay();

	if (!MainMenuHUDClass)
	{
		UE_LOG(PCLog, Warning, TEXT("MainMenuHUDClass Is NULL !!"));
		return;
	}
	MainMenu = CreateWidget<UMainMenuHUD>(this, MainMenuHUDClass, TEXT("MainMenuHUD"));
	MainMenu->AddToViewport();
	SetMouseInputMode(EInputModeType::E_UIOnly, MainMenu);
	UE_LOG(PCLog, Warning, TEXT("Create Widget call"));
}
