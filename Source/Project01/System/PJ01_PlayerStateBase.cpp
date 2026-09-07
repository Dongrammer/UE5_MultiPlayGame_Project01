#include "System/PJ01_PlayerStateBase.h"
#include "Net/UnrealNetwork.h"
#include "PC_Stage.h"

void APJ01_PlayerStateBase::Server_SetPlayerReady_Implementation(bool bReady)
{
	bIsPlayerReady = bReady;
	OnRep_PlayerReady();
}

void APJ01_PlayerStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APJ01_PlayerStateBase, bIsPlayerReady);
}

void APJ01_PlayerStateBase::OnRep_PlayerReady()
{
	UE_LOG(LogTemp, Warning, TEXT("OnRep_PlayerReady : %s"), *this->GetName());
	APlayerController* PC = Cast<APlayerController>(GetOwner());
	if (!PC) return;
	APC_Stage* PCS = Cast<APC_Stage>(PC);
	if (!PCS) return;

	if (bIsPlayerReady) PCS->EndLoading();
}
