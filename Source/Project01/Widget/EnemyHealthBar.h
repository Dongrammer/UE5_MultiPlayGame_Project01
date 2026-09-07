#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBar.generated.h"

class UProgressBar;

UCLASS()
class PROJECT01_API UEnemyHealthBar : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UProgressBar> PB_HP;

public:
	UFUNCTION(BlueprintCallable)
	void UpdateHPBar(float Current, float Max);
};
