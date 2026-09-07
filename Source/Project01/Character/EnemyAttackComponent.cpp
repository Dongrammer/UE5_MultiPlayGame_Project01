#include "Character/EnemyAttackComponent.h"
#include "Character/BaseCharacter.h"
// Targeting
#include "Kismet/KismetSystemLibrary.h"
#include "Hero/Hero.h"


void UEnemyAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	AttackRange = 3000.0f;
}

void UEnemyAttackComponent::SelectAttackTarget()
{
	Super::SelectAttackTarget();

	if (!OwnerCharacter || !OwnerCharacter->HasAuthority()) return;

	TArray<AActor*> Overlaps;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
	UKismetSystemLibrary::SphereOverlapActors(this, OwnerCharacter->GetActorLocation(), AttackRange, ObjTypes, AHero::StaticClass(), {OwnerCharacter}, Overlaps);

	if (Overlaps.Num() == 0) return;

	AActor* target = nullptr;
	float targetdis = TNumericLimits<float>::Max();

	for (AActor* actor : Overlaps)
	{
		const float Dis = FVector::DistSquared2D(actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
		if (Dis < targetdis)
		{
			targetdis = Dis;
			target = actor;
		}
	}

	if (!target) return;

	TObjectPtr<ABaseCharacter> c = Cast<ABaseCharacter>(target);
	if (!c)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("Target Casting Fail !!"));
		return;
	}
	if (AttackTarget != target)
	{
		AttackTarget = c;
		OnRep_AttackTarget();
	}
}

void UEnemyAttackComponent::DoAttack()
{
	Super::DoAttack();
}

void UEnemyAttackComponent::FireProjectileNotify()
{
	Super::FireProjectileNotify();
}

void UEnemyAttackComponent::EndAttackNotify()
{
	Super::EndAttackNotify();
}
