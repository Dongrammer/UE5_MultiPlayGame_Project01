#include "Widget/EnemyHealthBar.h"
#include "Components/ProgressBar.h"

void UEnemyHealthBar::UpdateHPBar(float Current, float Max)
{
	if (PB_HP)
		PB_HP->SetPercent(Current / Max);
}
