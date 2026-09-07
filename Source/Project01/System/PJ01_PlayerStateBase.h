#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "PJ01_PlayerStateBase.generated.h"

UCLASS()
class PROJECT01_API APJ01_PlayerStateBase : public APlayerState
{
	GENERATED_BODY()
	
public:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_PlayerReady, BlueprintReadOnly)
	bool bIsPlayerReady = false;

	UFUNCTION(Server, Reliable)
	void Server_SetPlayerReady(bool bReady);
	UFUNCTION()
	void OnRep_PlayerReady();
};
