#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "MainMenuCharacter.generated.h"

UCLASS()
class PROJECT01_API AMainMenuCharacter : public APawn
{
	GENERATED_BODY()

public:
	AMainMenuCharacter();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
