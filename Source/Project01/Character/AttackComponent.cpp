#include "Character/AttackComponent.h"
#include "Character/BaseCharacter.h"
#include "Net/UnrealNetwork.h"
#include "Character/Hero/Hero.h"
#include "Hero/EquipComponent.h"


DEFINE_LOG_CATEGORY(AttackCompLog);

UAttackComponent::UAttackComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	SetIsReplicatedByDefault(true);
}


void UAttackComponent::BeginPlay()
{
	Super::BeginPlay();
	OwnerCharacter = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerCharacter)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("OwnerCharacter Is NULL !!"));
		return;
	}
}

void UAttackComponent::SelectAttackTarget()
{
}

void UAttackComponent::Server_SetAttackTarget_Implementation(ABaseCharacter* target)
{
	SetAttackTarget(target);
}

void UAttackComponent::SetAttackTarget(ABaseCharacter* target)
{
	if (!OwnerCharacter) return;
	if (OwnerCharacter->HasAuthority())
	{
		AttackTarget = target;
		OnRep_AttackTarget();
	}
	else
	{
		Server_SetAttackTarget(target);
	}
}

void UAttackComponent::OnRep_AttackTarget()
{
	if (!GetWorld()->GetTimerManager().IsTimerActive(AttackTimer))
	{
		if (bCanNextAttack)
		{
			DoAttack();
		}
	}
}

void UAttackComponent::DoAttack()
{
	if (!OwnerCharacter || !AttackTarget) return;

	if (OwnerCharacter->HasAuthority())
	{
		if (DDoAttack.IsBound()) DDoAttack.Broadcast();
	}
}

void UAttackComponent::SetNextAttack()
{
	if (bAutoAttack) 
		GetWorld()->GetTimerManager().SetTimer(AttackTimer, this, &UAttackComponent::DoAttack, AttackCooldown, false);
}

void UAttackComponent::SetAttackRate(float rate)
{
	if (OwnerCharacter->HasAuthority())
	{
		AttackRate = FMath::Max(0.05f, rate);

		float FixRate = 0.7f; // The higher the value, the faster the animation speed
		AttackCooldown = 1.0f + (AttackRate - 1.0f) * (1.0f - FixRate); // 반올림 필요한가?
		AttackSpeed = 1.0f / (1.0f + (AttackRate - 1.0f) * FixRate);

		OnRep_AttackRate();
	}
	else
	{
		Server_SetAttackRate(rate);
	}
}

void UAttackComponent::Server_SetAttackRate_Implementation(float rate)
{
	SetAttackRate(rate);
}

void UAttackComponent::OnRep_AttackRate()
{
}

void UAttackComponent::FireProjectileNotify()
{
	if (!OwnerCharacter->HasAuthority()) return;
}

void UAttackComponent::EndAttackNotify()
{
	if (!bAutoAttack) return;
	if (OwnerCharacter->HasAuthority()) SetNextAttack(); 
	bCanNextAttack = true;
}

void UAttackComponent::StartAutoAttack()
{
	DoAttack();
	bAutoAttack = true;
}

void UAttackComponent::StopAutoAttack()
{
	GetWorld()->GetTimerManager().ClearTimer(AttackTimer);
	bAutoAttack = false;
}

void UAttackComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UAttackComponent, AttackTarget);
	DOREPLIFETIME(UAttackComponent, AttackRate);
}

