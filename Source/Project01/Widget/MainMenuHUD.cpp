#include "Widget/MainMenuHUD.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuHUD::NativeConstruct()
{
	Super::NativeConstruct();

	FScriptDelegate SinglePlayPressed;
	SinglePlayPressed.BindUFunction(this, "SinglePlayStart");
	BT_SinglePlay->OnClicked.Add(SinglePlayPressed);
	FScriptDelegate MultiPlayPressed;
	MultiPlayPressed.BindUFunction(this, "MultiPlayStart");
	BT_MultiPlay->OnClicked.Add(MultiPlayPressed);
	FScriptDelegate ForgePressed;
	ForgePressed.BindUFunction(this, "ForgeStart");
	BT_Forge->OnClicked.Add(ForgePressed);
	FScriptDelegate PetPressed;
	PetPressed.BindUFunction(this, "PetStart");
	BT_Pet->OnClicked.Add(PetPressed);
	FScriptDelegate OptionPressed;
	OptionPressed.BindUFunction(this, "OptionStart");
	BT_Option->OnClicked.Add(OptionPressed);

}

void UMainMenuHUD::SinglePlayStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Single Play Open"));
	UGameplayStatics::OpenLevel(this, TEXT("Level_Stage"));
}

void UMainMenuHUD::MultiPlayStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Multi Play Open"));
}

void UMainMenuHUD::ForgeStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Forge Open"));
}

void UMainMenuHUD::PetStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Pet Open"));
}

void UMainMenuHUD::OptionStart()
{
	UE_LOG(LogTemp, Warning, TEXT("Option Open"));
}
