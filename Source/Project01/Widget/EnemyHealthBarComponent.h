#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "EnemyHealthBarComponent.generated.h"

class UProgressBar;

UCLASS()
class PROJECT01_API UEnemyHealthBarComponent : public UWidgetComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> PB_HP;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateHPBar(float Current, float Max);
};
