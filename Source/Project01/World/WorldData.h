#pragma once

#include "CoreMinimal.h"
#include "WorldData.generated.h"

UENUM(BlueprintType)
enum class EGamePhase : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Loading UMETA(DisplayName = "Loading"),
	E_Waiting UMETA(DisplayName = "Waiting"),
	E_Running UMETA(DIsplayName = "Running"),
	E_Pause UMETA(DisplayName = "Pause")
};

UENUM(BlueprintType)
enum class EEnemyType : uint8
{
	E_None UMETA(DisplayName = "None"),
	E_Slime UMETA(DisplayName = "Slime"),
	E_Mushroom UMETA(DisplayName = "Mushroom")
};

USTRUCT(BlueprintType)
struct FStageEnemyStatusTable : public FTableRowBase // If you have modified this stucture, make sure that the engine generates the DataTable again
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int StageTime = 180;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float WorldSpeed = 200.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int EnemyCount = 10;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMin = 1.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EnemySpawnIntervalMax = 2.0f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float NamedUpgradeRate = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TMap<EEnemyType, int> EnemyComposition; // 1000 = 100.0%
};