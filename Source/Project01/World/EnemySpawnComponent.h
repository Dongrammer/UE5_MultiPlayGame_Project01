#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"

#include "WorldData.h"

#include "EnemySpawnComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class PROJECT01_API UEnemySpawnComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UEnemySpawnComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void InitStatus(FStageEnemyStatusTable* table);
	void StartSpawn();
	void EnemySpawn();
private:
	FStageEnemyStatusTable* EnemyStatus;

	FTimerHandle SpawnHandle;
};
