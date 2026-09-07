#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "DebugSubsystem.generated.h"

UCLASS()
class PROJECT01_API UDebugSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bAttackDebugMode = false;
};
