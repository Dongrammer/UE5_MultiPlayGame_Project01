#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemySpawnArea.generated.h"

class UBoxComponent;

UCLASS()
class PROJECT01_API AEnemySpawnArea : public AActor
{
	GENERATED_BODY()
	
public:	
	AEnemySpawnArea();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TObjectPtr<UBoxComponent> BoxComponent;

public:
	void SetBoxSize(FVector size);
	FVector GetRandomPoint() const;

};
