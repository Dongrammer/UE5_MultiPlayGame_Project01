#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "StartCountHUD.generated.h"

class UCanvasPanel;
class USizeBox;
class UOverlay;
class UTextBlock;

UCLASS()
class PROJECT01_API UStartCountHUD : public UUserWidget
{
	GENERATED_BODY()

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CP_01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SB_01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UOverlay> OL_Text;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TB_Ready;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TB_Start;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> WA_Ready;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidgetAnim, AllowPrivateAccess = "true"), Transient)
	TObjectPtr<UWidgetAnimation> WA_Start;

public:
	void PlayReadyAnim();
	void PlayStartAnim();
};
