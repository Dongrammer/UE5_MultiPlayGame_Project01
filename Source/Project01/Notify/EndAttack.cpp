#include "Notify/EndAttack.h"
#include "Character/BaseCharacter.h"
#include "Character/AttackComponent.h"

void UEndAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	TObjectPtr<ABaseCharacter> OwnerCharacter = Cast<ABaseCharacter>(MeshComp->GetOwner());

	if (!OwnerCharacter) return;
	TObjectPtr<UAttackComponent> AttackComponent = OwnerCharacter->GetAttackComponent();
	if (!AttackComponent) return;
	AttackComponent->EndAttackNotify();
}
