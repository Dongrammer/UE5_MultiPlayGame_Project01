#include "System/PJ01_PlayerControllerBase.h"
#include "Blueprint/UserWidget.h"

void APJ01_PlayerControllerBase::SetMouseInputMode(EInputModeType mode, UUserWidget* widget)
{
	switch (mode)
	{
	case EInputModeType::E_UIOnly:
	{
		FInputModeUIOnly Mode; 
		
		if (widget) Mode.SetWidgetToFocus(widget->TakeWidget());
		Mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
		SetInputMode(Mode);
		bShowMouseCursor = true;
		break;
	}
	case EInputModeType::E_GameOnly:
	{
		FInputModeGameOnly Mode;
		SetInputMode(Mode);
		bShowMouseCursor = true;
		break;
	}
	case EInputModeType::E_GameAndUI:
	{
		FInputModeGameAndUI Mode;
		SetInputMode(Mode);
	}
	default:
		break;
	}
}
