#pragma once

#include "CoreMinimal.h"
#include "System/PJ01_PlayerControllerBase.h"
#include "PC_Main.generated.h"

class UMainMenuHUD;

UCLASS()
class PROJECT01_API APC_Main : public APJ01_PlayerControllerBase
{
	GENERATED_BODY()
	
public:
	APC_Main();

	void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TSubclassOf<UMainMenuHUD> MainMenuHUDClass;
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UMainMenuHUD> MainMenu;

};
