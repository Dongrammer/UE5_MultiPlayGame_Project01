#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InputDataAsset.generated.h"

class UInputAction;

UCLASS()
class PROJECT01_API UInputDataAsset : public UDataAsset
{
	GENERATED_BODY()

	/* ==================== Movement  ====================*/
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> MoveInput;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> JumpAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UInputAction> TurnAction;
};
