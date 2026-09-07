#include "PJ01_GameStateBase.h"
#include "Net/UnrealNetwork.h"
#include "World/WorldManager.h"
#include "Materials/MaterialParameterCollection.h"
#include "Materials/MaterialParameterCollectionInstance.h"
#include "Helper.h"

DEFINE_LOG_CATEGORY(GameStateLog);

void APJ01_GameStateBase::BeginPlay()
{
	Super::BeginPlay();

	MPCAsset = Helper::GetAssetDynamic<UMaterialParameterCollection>(TEXT("/Game/World/MPC_World"));
	if (!MPCAsset)
	{
		UE_LOG(GameStateLog, Warning, TEXT("MPCAsset Is NULL !!"));
		return;
	}
}

void APJ01_GameStateBase::OnRep_WorldManager()
{
	DOnWorldManagerReady.Broadcast(WorldManager);
}

void APJ01_GameStateBase::SetWorldManager(AWorldManager* manager)
{
	WorldManager = manager;
	OnRep_WorldManager();

	if (!WorldManager)
	{
		UE_LOG(GameStateLog, Warning, TEXT("WorldManager Is NULL !!"));
		return;
	}
}

void APJ01_GameStateBase::StartStage(FStageEnemyStatusTable* data)
{
	if (!HasAuthority()) return;
	if (!WorldManager)
	{
		UE_LOG(GameStateLog, Warning, TEXT("World Manager Is NULL !!"));
		return;
	}

	EnemyTable = data;
	RemainingEnemies = EnemyTable->EnemyCount;

	WorldManager->DWorldSpeedChanged.AddUFunction(this, TEXT("WorldSpeedChanged")); // WorldSpeed Binding
	WorldManager->SetWorldSpeed(EnemyTable->WorldSpeed);
	WorldManager->StartWorld();

	SetGamePhase(EGamePhase::E_Running);
	GetWorld()->GetTimerManager().SetTimer(EnemySpawnHandle, this, &APJ01_GameStateBase::SpawnEnemy, 3.0f, false);
}

void APJ01_GameStateBase::PauseStage()
{
	if (!WorldManager)
	{
		UE_LOG(GameStateLog, Warning, TEXT("World Manager Is NULL !!"));
		return;
	}

	WorldManager->StopWorld();
}

void APJ01_GameStateBase::SpawnEnemy()
{
	if (!HasAuthority()) return;

	if (RemainingEnemies <= 0) return;
	RemainingEnemies--;

	// Spawn Interval
	float Interval = FMath::FRandRange(EnemyTable->EnemySpawnIntervalMin, EnemyTable->EnemySpawnIntervalMax);
	// Named Change Rate
	bool named = FMath::RandRange(1, 100) <= EnemyTable->NamedUpgradeRate;
	// Enemy Type
	EEnemyType type = EEnemyType::E_None;
	int rand = FMath::RandRange(1, 1000);
	int EValue = 0;
	for (auto& enemy : EnemyTable->EnemyComposition)
	{
		EValue += enemy.Value;
		if (EValue <= rand)
		{
			type = enemy.Key;
			break;
		}
	}
	WorldManager->EnemySpawn(type, named);

	GetWorld()->GetTimerManager().SetTimer(EnemySpawnHandle, this, &APJ01_GameStateBase::SpawnEnemy, Interval, false);
}

void APJ01_GameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APJ01_GameStateBase, GamePhase);
	DOREPLIFETIME(APJ01_GameStateBase, WorldManager);
	DOREPLIFETIME(APJ01_GameStateBase, MPCWorldSpeed);
}

void APJ01_GameStateBase::SetGamePhase(EGamePhase NewPhase)
{
	if (HasAuthority() && GamePhase != NewPhase)
	{
		GamePhase = NewPhase;
		OnRep_GamePhase();
	}
}

void APJ01_GameStateBase::OnRep_GamePhase()
{
	DOnGamePhaseChanged.Broadcast(GamePhase);
}

void APJ01_GameStateBase::WorldSpeedChanged(float speed)
{
	UE_LOG(LogTemp, Warning, TEXT("WorldSpeedChanged Call : %0.2f"), speed);
	if (!HasAuthority()) return;
	MPCWorldSpeed = speed;
	OnRep_MPCValues();
}

void APJ01_GameStateBase::OnRep_MPCValues()
{
	if (!MPCAsset) return;
	TObjectPtr<UMaterialParameterCollectionInstance> MPCI = GetWorld()->GetParameterCollectionInstance(MPCAsset);
	if (!MPCI)
	{
		UE_LOG(GameStateLog, Warning, TEXT("MPCI Is NULL !!"));
		return;
	}
	MPCI->SetScalarParameterValue(TEXT("WorldSpeed"), MPCWorldSpeed);
	UE_LOG(LogTemp, Warning, TEXT("MPCWorldSpeed : %0.2f"), MPCWorldSpeed);
}
