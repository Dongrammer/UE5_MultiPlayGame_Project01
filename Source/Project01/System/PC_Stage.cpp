#include "System/PC_Stage.h"
#include "Helper.h"
#include "Widget/MainHUD.h"
#include "Widget/LoadingHUD.h"
#include "Widget/StartCountHUD.h"
#include "Net/UnrealNetwork.h"

APC_Stage::APC_Stage()
{
	MainHUDClass = Helper::GetClass<UMainHUD>(TEXT("/Game/Widget/BP_MainHUD"));
	LoadingHUDClass = Helper::GetClass<ULoadingHUD>(TEXT("/Game/Widget/BP_LoadingHUD"));
	StartCountHUDClass = Helper::GetClass<UStartCountHUD>(TEXT("/Game/Widget/BP_StartCountHUD"));
}

void APC_Stage::BeginPlay()
{
	Super::BeginPlay();

	SetMouseInputMode(EInputModeType::E_GameOnly);

	if (!IsLocalController()) return;

	/* ========== Widget ========== */
	{
		if (!MainHUDClass)
		{
			UE_LOG(PCLog, Warning, TEXT("MainHUDClass Is NULL !!"));
			return;
		}

		MainHUD = CreateWidget<UMainHUD>(this, MainHUDClass, TEXT("Main HUD"));
		MainHUD->AddToViewport();
		MainHUD->InitRunningTime();

		if (!LoadingHUDClass)
		{
			UE_LOG(PCLog, Warning, TEXT("LoadingHUDClass Is NULL !!"));
			return;
		}

		LoadingHUD = CreateWidget<ULoadingHUD>(this, LoadingHUDClass, TEXT("Loading HUD"));
		LoadingHUD->AddToViewport();

		if (!StartCountHUDClass)
		{
			UE_LOG(PCLog, Warning, TEXT("StartCountHUDClass Is NULL !!"));
			return;
		}

		StartCountHUD = CreateWidget<UStartCountHUD>(this, StartCountHUDClass, TEXT("StartCount HUD"));
	}

	Server_SetControllerReady(true);
}

void APC_Stage::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APC_Stage, bIsControllerReady);
}

void APC_Stage::EndLoading()
{
	if (!IsLocalController()) return;
	if (LoadingHUD) LoadingHUD->RemoveFromParent();
	if (!StartCountHUD) return;
	if (!StartCountHUD->IsInViewport()) StartCountHUD->AddToViewport();
	StartCountHUD->PlayReadyAnim();
}

void APC_Stage::Server_SetControllerReady_Implementation(bool b)
{
	bIsControllerReady = b;
}

void APC_Stage::Multicast_StartStage_Implementation()
{
	if (!IsLocalController()) return;
	if (!StartCountHUD) return;
	if (LoadingHUD) LoadingHUD->RemoveFromParent();
	if (!StartCountHUD->IsInViewport()) StartCountHUD->AddToViewport();

	StartCountHUD->PlayStartAnim();
}
