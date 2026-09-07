#include "Animation/AnimInstanceBase.h"

#include "Character/BaseCharacter.h"

DEFINE_LOG_CATEGORY(AnimInstanceLog);

void UAnimInstanceBase::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ABaseCharacter>(TryGetPawnOwner());

	if (Owner != nullptr)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void UAnimInstanceBase::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (!Owner) return;
	UpdateBlendSpaceVariable();
}

void UAnimInstanceBase::UpdateBlendSpaceVariable()
{
	if (!Movement || !Owner)
	{
		UE_LOG(AnimInstanceLog, Warning, TEXT("Owner or Movement Is NULL !!"));
		return;
	}

	Speed = Owner->GetSpeed();
	bShouldMove = Owner->GetShouldMove();
	AttackSpeed = Owner->GetAttackSpeed();
	bShouldAttack = Owner->GetShouldAttack();
}
