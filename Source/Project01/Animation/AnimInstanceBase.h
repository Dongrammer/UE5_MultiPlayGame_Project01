#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"

DECLARE_LOG_CATEGORY_EXTERN(AnimInstanceLog, Log, All);

#include "AnimInstanceBase.generated.h"

class ABaseCharacter;
class UCharacterMovementComponent;

UCLASS()
class PROJECT01_API UAnimInstanceBase : public UAnimInstance
{
	GENERATED_BODY()

public:
	virtual void NativeInitializeAnimation() override;
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

protected:
	virtual void UpdateBlendSpaceVariable();

protected:
	UPROPERTY(BlueprintReadOnly, Category = "BlendSpace", meta = (AllowPrivateAccess = "true"))
	bool bShouldMove;

	UPROPERTY(BlueprintReadOnly, Category = "BlendSpace", meta = (AllowPrivateAccess = "true"))
	float Speed;

	UPROPERTY(BlueprintReadOnly, Category = "BlendSpace", meta = (AllowPrivateAccess = "true"))
	bool bShouldAttack;

	UPROPERTY(BlueprintReadOnly, Category = "BlendSpace", meta = (AllowPrivateAccess = "true"))
	float AttackSpeed;

protected:
	TObjectPtr<ABaseCharacter> Owner;

	TObjectPtr<UCharacterMovementComponent> Movement;
};
