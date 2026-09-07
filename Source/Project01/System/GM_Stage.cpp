#include "System/GM_Stage.h"
#include "Helper.h"
#include "PC_Stage.h"
#include "World/WorldManager.h"
#include "World/WorldData.h"
#include "PJ01_GameStateBase.h"
#include "PJ01_PlayerStateBase.h"

AGM_Stage::AGM_Stage()
{
	TSubclassOf<APawn> PlayerPawnClass = Helper::GetClass<APawn>(TEXT("/Game/Character/Hero/BP_Hero"));
	if (PlayerPawnClass != NULL)
		DefaultPawnClass = PlayerPawnClass;

	TSubclassOf<APC_Stage> PlayerContClass = APC_Stage::StaticClass();
	if (PlayerContClass != NULL)
		PlayerControllerClass = PlayerContClass;

	TSubclassOf<AWorldManager> WManager = Helper::GetClass<AWorldManager>(TEXT("/Game/World/BP_WorldManager"));
	if (WManager != NULL)
		WorldManagerClass = WManager;

	TSubclassOf<APJ01_PlayerStateBase> PStateClass = APJ01_PlayerStateBase::StaticClass();
	if (PStateClass != NULL)
		PlayerStateClass = PStateClass;
}

void AGM_Stage::StartPlay()
{
	Super::StartPlay();

	// World Manager Spawn and Setting
	if (!WorldManagerClass)
	{
		UE_LOG(GameModeLog, Warning, TEXT("WorldManagerClass Is NULL !!"));
		return;
	}

	if (!WorldManager)
	{
		FActorSpawnParameters asp;
		asp.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		FTransform transform = FTransform::Identity;
		WorldManager = GetWorld()->SpawnActor<AWorldManager>(WorldManagerClass, transform, asp);
		if (!WorldManager)
		{
			UE_LOG(GameModeLog, Warning, TEXT("WorldManager Is Not Created !!"));
		}
	}

	MyGameState = GetGameState<APJ01_GameStateBase>();
	if (!MyGameState)
	{
		UE_LOG(GameModeLog, Warning, TEXT("GameState Is NULL !!"));
		return;
	}
	MyGameState->SetWorldManager(WorldManager);
	GetWorld()->GetTimerManager().SetTimer(THPlayerReady, this, &AGM_Stage::CheckPlayerReady, 0.5f, true);

	StageEnemyStatusTable = Helper::GetAssetDynamic<UDataTable>(TEXT("/Game/Stage/DT_StageEnemyStatus.DT_StageEnemyStatus"));
	if (!StageEnemyStatusTable)
	{
		UE_LOG(GameModeLog, Warning, TEXT("StageEnemyStatusTable Is NULL !!"));
		return;
	}
}

void AGM_Stage::StartStage()
{
	if (!MyGameState)
	{
		UE_LOG(GameModeLog, Warning, TEXT("GameStage Is NULL !!"));
		return;
	}

	FName RowName = *FString::Printf(TEXT("Stage_%s"), *CurrentStage);
	FStageEnemyStatusTable* data = StageEnemyStatusTable->FindRow<FStageEnemyStatusTable>(RowName, TEXT("Stage ID"), true);

	if (!data)
	{
		UE_LOG(GameModeLog, Warning, TEXT("StageTable Row Read Fail !! : %s"), *RowName.ToString());
		return;
	}

	// Player Start HUD
	for (auto& players : MyGameState->PlayerArray)
	{
		APC_Stage* PCS = Cast<APC_Stage>(players->GetOwner());
		if (!PCS) return;
		PCS->Multicast_StartStage();
	}

	MyGameState->StartStage(data);
}

void AGM_Stage::PauseStage()
{
	if (!MyGameState)
	{
		UE_LOG(GameModeLog, Warning, TEXT("GameStage Is NULL !!"));
		return;
	}
	MyGameState->PauseStage();
}

void AGM_Stage::CheckPlayerReady()
{
	if (!MyGameState) return;
	// Player Count Check
	if (ConnectPlayer > MyGameState->PlayerArray.Num()) return;

	// Player Ready Check
	for (auto& players : MyGameState->PlayerArray)
	{
		APJ01_PlayerStateBase* PS = Cast<APJ01_PlayerStateBase>(players);
		if (!PS || !PS->bIsPlayerReady) return;
	}
	GetWorld()->GetTimerManager().ClearTimer(THPlayerReady);

	// Stage Start
	FTimerHandle THStartGame;
	GetWorld()->GetTimerManager().SetTimer(THStartGame, this, &AGM_Stage::StartStage, 2.0f);
}
