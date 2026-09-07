#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LoadingHUD.generated.h"

class UCanvasPanel;
class USizeBox;
class UImage;

UCLASS()
class PROJECT01_API ULoadingHUD : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CP_01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SB_01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UImage> IM_Loading;
};
