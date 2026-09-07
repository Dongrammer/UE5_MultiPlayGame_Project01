#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainHUD.generated.h"

class UCanvasPanel;
class USizeBox;
class UTextBlock;

UCLASS()
class PROJECT01_API UMainHUD : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CP_01;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SB_RunTime;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget), meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UTextBlock> TB_RunTime;

public:
	virtual void NativeConstruct() override;
	
	void InitRunningTime();
	void SetWorldRunningTime(int time);
};
