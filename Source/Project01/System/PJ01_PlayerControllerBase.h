#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PJ01_PlayerControllerBase.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(PCLog, Log, All);

class UUserWIdget;

UENUM(BlueprintType)
enum class EInputModeType : uint8
{
	E_UIOnly,
	E_GameOnly,
	E_GameAndUI
};

UCLASS()
class PROJECT01_API APJ01_PlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
protected:
	UFUNCTION(BlueprintCallable)
	virtual void SetMouseInputMode(EInputModeType mode, UUserWidget* widget = nullptr);
};
