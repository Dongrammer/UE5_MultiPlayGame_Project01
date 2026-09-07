#pragma once

#include "CoreMinimal.h"
#include "System/PJ01_PlayerControllerBase.h"
#include "PC_Stage.generated.h"

class UMainHUD;
class ULoadingHUD;
class UStartCountHUD;

UCLASS()
class PROJECT01_API APC_Stage : public APJ01_PlayerControllerBase
{
	GENERATED_BODY()
	

public:
	APC_Stage();
	void BeginPlay() override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainHUD> MainHUDClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<ULoadingHUD> LoadingHUDClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UStartCountHUD> StartCountHUDClass;
	
private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainHUD> MainHUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<ULoadingHUD> LoadingHUD;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStartCountHUD> StartCountHUD;

	UPROPERTY(Replicated, EditDefaultsOnly, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	bool bIsControllerReady = false;

public:
	TObjectPtr<UMainHUD> GetMainHUD() const { return MainHUD; }
	void EndLoading();
	UFUNCTION(NetMulticast, Reliable)
	void Multicast_StartStage();
	UFUNCTION(Server, Reliable)
	void Server_SetControllerReady(bool b);
	bool GetControllerReady() const { return bIsControllerReady; }
};
