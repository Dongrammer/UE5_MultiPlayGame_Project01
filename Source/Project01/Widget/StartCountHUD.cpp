#include "Widget/StartCountHUD.h"
#include "Components/CanvasPanel.h"
#include "Components/SizeBox.h"
#include "Components/Overlay.h"
#include "Components/TextBlock.h"

void UStartCountHUD::PlayReadyAnim()
{
	PlayAnimation(WA_Ready);
}

void UStartCountHUD::PlayStartAnim()
{
	PlayAnimation(WA_Start);
}
