// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PJ01_GameModeBase.h"
#include "GM_Stage.generated.h"

class AWorldManager;
class APJ01_GameStateBase;

UCLASS()
class PROJECT01_API AGM_Stage : public APJ01_GameModeBase
{
	GENERATED_BODY()
	
protected: 
	AGM_Stage();

	void StartPlay() override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AWorldManager> WorldManagerClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<AWorldManager> WorldManager;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<APJ01_GameStateBase> MyGameState;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Essential", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UDataTable> StageEnemyStatusTable;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	FString CurrentStage = "01001";

	int ConnectPlayer = 2;
public:
	UFUNCTION(BlueprintCallable)
	void StartStage();
	UFUNCTION(BlueprintCallable)
	void PauseStage();

private:
	FTimerHandle THPlayerReady;
	void CheckPlayerReady();
};
