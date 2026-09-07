#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "PJ01_GameInstance.generated.h"

UCLASS()
class PROJECT01_API UPJ01_GameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	virtual void Init() override;
	virtual void OnStart() override;
};
