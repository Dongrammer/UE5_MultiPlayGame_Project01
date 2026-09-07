#include "World/WorldManager.h"
#include "Net/UnrealNetwork.h"
#include "Helper.h"

#include "EnemySpawnComponent.h"
#include "EnemySpawnArea.h"
#include "Character/Enemy/EnemyCharacter.h"
#include "Character/Enemy/EnemyCatalogDataAsset.h"
#include "EngineUtils.h"

DEFINE_LOG_CATEGORY(WorldManagerLog);

AWorldManager::AWorldManager()
{
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	bAlwaysRelevant = true;
	bOnlyRelevantToOwner = false;
	// NetUpdateFrequency = 10.f; // 상태 변화 잦으면 올리고, 적으면 내려도 됨
	// NetPriority = 3.f; // 중요도 소폭 올림

	EnemySpawnComponent = Helper::CreateActorComponent<UEnemySpawnComponent>(this, "EnemySpawn Component");
}

void AWorldManager::BeginPlay()
{
	Super::BeginPlay();

	SpawnAreaFront = GetWorld()->SpawnActor<AEnemySpawnArea>(SpawnAreaClass, FrontSpawnLocation, FRotator::ZeroRotator);
	SpawnAreaBack = GetWorld()->SpawnActor<AEnemySpawnArea>(SpawnAreaClass, BackSpawnLocation, FRotator::ZeroRotator);

	SpawnAreaFront->SetBoxSize(SpawnAreaSize);
	/*for(TActorIterator<AEnemySpawnArea> It(GetWorld()); It; ++It)
	{
		if (It->GetName().Contains("Front"))
		{
			UE_LOG(LogTemp, Warning, TEXT("DOne"));
			SpawnAreaFront = *It;
		}
		else if (It->GetName().Contains("Back")) SpawnAreaBack = *It;
	}*/
}

void AWorldManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWorldManager::StartWorld()
{
	if (!HasAuthority()) return;
	if (GetWorld()->GetTimerManager().IsTimerPaused(THandle_ElapsedTime))
	{
		GetWorld()->GetTimerManager().UnPauseTimer(THandle_ElapsedTime);
		return;
	}
	Server_ElapsedTimeStart();
}

void AWorldManager::StopWorld()
{
	if (!HasAuthority()) return;
	GetWorld()->GetTimerManager().PauseTimer(THandle_ElapsedTime);
}

void AWorldManager::SetWorldSpeed(float NewSpeed)
{
	if (HasAuthority()) // 서버일때
	{
		if (!FMath::IsNearlyEqual(WorldSpeed, NewSpeed))
		{
			WorldSpeed = NewSpeed;
			ApplyWorldSpeed();
		}
	}
	else
	{
		Server_SetWorldSpeed(NewSpeed);
	}
}

void AWorldManager::ApplyWorldSpeed()
{
	if(DWorldSpeedChanged.IsBound()) DWorldSpeedChanged.Broadcast(WorldSpeed);
}

void AWorldManager::TickElapsedTime()
{
	ElapsedTime++; 
	OnRep_ElapsedTime();
}

void AWorldManager::StartEnemySpawn(FStageEnemyStatusTable* data)
{
	if (!HasAuthority()) return;
	if (!EnemySpawnComponent) return;

	EnemySpawnComponent->InitStatus(data);
	EnemySpawnComponent->StartSpawn();
}

void AWorldManager::EnemySpawn(EEnemyType type, bool bnamed)
{
	if (!HasAuthority()) return;

	// Enemy Class
	TSubclassOf<AEnemyCharacter> EnemyClass = EnemyCatalog->GetEnemyClass(type, bnamed);
	if (!EnemyClass)
	{
		UE_LOG(WorldManagerLog, Warning, TEXT("EnemyClass Is NULL !!"));
		return;
	}

	// Spawn Point
	if (!SpawnAreaFront || !SpawnAreaBack)
	{
		UE_LOG(WorldManagerLog, Warning, TEXT("SpawnArea Is NULL !!"));
		return;
	}
	FVector SpawnPoint = FVector::ZeroVector;
	bool FE = FMath::RandBool();
	if (FE) SpawnPoint = SpawnAreaFront->GetRandomPoint();
	else SpawnPoint = SpawnAreaBack->GetRandomPoint();

	TObjectPtr<AEnemyCharacter> enemy = GetWorld()->SpawnActor<AEnemyCharacter>(EnemyClass, SpawnPoint, FRotator::ZeroRotator);
	if (enemy)
	{
		enemy->SetFrontBack(FE);
	}

}

void AWorldManager::AddHero(ABaseCharacter* hero)
{
	Heros.AddUnique(hero);
}

TArray<TObjectPtr<ABaseCharacter>> AWorldManager::GetHeros()
{
	return Heros;
}

void AWorldManager::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AWorldManager, WorldSpeed);
	DOREPLIFETIME(AWorldManager, ElapsedTime);
}

void AWorldManager::Server_SetWorldSpeed_Implementation(float NewSpeed)
{
	SetWorldSpeed(NewSpeed);
}

void AWorldManager::Server_ElapsedTimeStart_Implementation()
{
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(THandle_ElapsedTime, this, &AWorldManager::TickElapsedTime, 1.0f, true, 1.0f);
	}
}

void AWorldManager::OnRep_WorldSpeed()
{
	ApplyWorldSpeed();
}

void AWorldManager::OnRep_ElapsedTime()
{
	DWorldTimeChanged.Broadcast(ElapsedTime);
}

