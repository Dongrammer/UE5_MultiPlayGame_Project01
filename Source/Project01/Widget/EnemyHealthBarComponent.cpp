#include "Widget/EnemyHealthBarComponent.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBarComponent::UpdateHPBar(float Current, float Max)
{
	if (PB_HP)
		PB_HP->SetPercent(Current / Max);
}


