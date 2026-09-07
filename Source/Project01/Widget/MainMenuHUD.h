#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuHUD.generated.h"

class UCanvasPanel;
class USizeBox;
class UButton;
class UVerticalBox;

UCLASS()
class PROJECT01_API UMainMenuHUD : public UUserWidget
{
	GENERATED_BODY()

protected:
	void NativeConstruct() override;

private:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UCanvasPanel> CP_01;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<USizeBox> SB_Menu;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UVerticalBox> VB_Menu;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BT_SinglePlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BT_MultiPlay;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BT_Forge;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BT_Pet;
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, meta = (BindWidget, AllowPrivateAccess = "true"))
	TObjectPtr<UButton> BT_Option; 

public:
	UFUNCTION()
	void SinglePlayStart();
	UFUNCTION()
	void MultiPlayStart();
	UFUNCTION()
	void ForgeStart();
	UFUNCTION()
	void PetStart();
	UFUNCTION()
	void OptionStart();
};
