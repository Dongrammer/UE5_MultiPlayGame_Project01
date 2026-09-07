#include "Widget/MainHUD.h"

#include "Components/SizeBox.h"
#include "Components/TextBlock.h"

void UMainHUD::NativeConstruct()
{
	Super::NativeConstruct();
}

void UMainHUD::InitRunningTime()
{
	TB_RunTime->SetText(FText::FromString(FString::Printf(TEXT("00 : 00"))));
}

void UMainHUD::SetWorldRunningTime(int time)
{
	int min = time / 60;
	int sec = time % 60;

	FString full = FString::Printf(TEXT("%02d : %02d"), min, sec);
	TB_RunTime->SetText(FText::FromString(full));
}
