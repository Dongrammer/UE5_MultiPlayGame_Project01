#include "Character/BaseCharacter.h"
#include "Helper.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

// Server
#include "Net/UnrealNetwork.h"
// Rotator
#include "Kismet/KismetMathLibrary.h"
// Attack
#include "Character/AttackComponent.h"
// Game
#include "System/PJ01_GameStateBase.h"
// World
#include "World/WorldManager.h"
#include "World/WorldData.h"

ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	TopRootComponent = GetCapsuleComponent();

	BodyCollision = Helper::CreateSceneComponent<UCapsuleComponent>(this, "BodyCollision", RootComponent);
	
	SetCharacterMovement();
	SetReplicates(true);
	SetReplicateMovement(true);

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	{
		const TObjectPtr<APJ01_GameStateBase> GameState = GetWorld()->GetGameState<APJ01_GameStateBase>();
		if (GameState)
		{
			GameState->DOnGamePhaseChanged.AddUFunction(this, "GamePhaseChanged");

			if (GameState->GetWorldManager()) WorldManagerSetting(GameState->GetWorldManager());
			else GameState->DOnWorldManagerReady.AddUFunction(this, "WorldManagerSetting");
		}
	}

	// ReTarget
	{
		if (!HasAuthority() || !AttackComponent) return;
		FTimerHandle TH;
		GetWorld()->GetTimerManager().SetTimer(TH, this, &ABaseCharacter::SelectTarget, RetargetInterval, true, 3.0f);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	RotateToTarget();
}

void ABaseCharacter::SetCharacterMovement()
{
	TObjectPtr<UCharacterMovementComponent> Comp = GetCharacterMovement();

	//Comp->bOrientRotationToMovement = true;
	Comp->RotationRate = FRotator(0.0f, 500.0f, 0.0f);

	Comp->JumpZVelocity = 700.0f;
	Comp->AirControl = 0.35f;
	Comp->MaxWalkSpeed = 600.0f;

	Comp->BrakingDecelerationWalking = 1000.0f;
	Comp->BrakingDecelerationFalling = 1500.0f;

	bUseControllerRotationYaw = false;
}

void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABaseCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseCharacter, WorldManager);
	DOREPLIFETIME(ABaseCharacter, bShouldMove);
	DOREPLIFETIME(ABaseCharacter, Speed);
	DOREPLIFETIME(ABaseCharacter, bInFront);
	DOREPLIFETIME(ABaseCharacter, HealthPoint);
	DOREPLIFETIME(ABaseCharacter, MaxHealthPoint);
	//DOREPLIFETIME(ABaseCharacter, TeamID);
}

void ABaseCharacter::WorldManagerSetting(AWorldManager* wm)
{
	if (!wm) return;
	WorldManager = wm;
	WorldManager->DWorldSpeedChanged.AddUFunction(this, "WorldSpeedChanged");

}

void ABaseCharacter::Server_StartRunning_Implementation()
{
	StartRunning();
}

void ABaseCharacter::StartRunning()
{
	if (!HasAuthority()) return;

	SetShouldMove(true);

	if (!AttackComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : AttackComponent Is NULL !!"), *GetName());
		return;
	}
	AttackComponent->StartAutoAttack();
}

void ABaseCharacter::GamePhaseChanged(EGamePhase phase)
{
	switch (phase)
	{
	case EGamePhase::E_Waiting:
	{
		break;
	}
	case EGamePhase::E_Running:
	{
		if (HasAuthority()) StartRunning();
		else Server_StartRunning();

		break;
	}
	default:
		break;
	}
}

void ABaseCharacter::WorldSpeedChanged(float NewSpeed)
{
	Speed = NewSpeed;
}

void ABaseCharacter::Server_SetFrontBack_Implementation(bool front)
{
	SetFrontBack(front);
}

void ABaseCharacter::SetFrontBack(bool front)
{
	if (HasAuthority())
	{
		bInFront = front;
		OnRep_InFront();
	}
	else
	{
		Server_SetFrontBack(front);
	}
}

void ABaseCharacter::OnRep_InFront()
{
}

void ABaseCharacter::RotateToTarget()
{
	if (!AttackComponent || !AttackComponent->GetAttackTarget()) return;

	// Rotation
	FRotator Rot = GetActorRotation();
	FRotator LookAtRot = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), AttackComponent->GetAttackTarget()->GetActorLocation());
	float YawDelta = FMath::FindDeltaAngleDegrees(Rot.Yaw, LookAtRot.Yaw);
	AddActorWorldRotation(FRotator(0, YawDelta, 0));
}

float ABaseCharacter::GetAttackSpeed() const
{
	if (!AttackComponent) return 1.0f;

	return AttackComponent->GetAttackRate();
}

void ABaseCharacter::SelectTarget()
{
	if (HasAuthority())
	{
		if (!AttackComponent) return;
		AttackComponent->SelectAttackTarget();
	}
	else
	{
		Server_SelectTarget();
	}
}

void ABaseCharacter::Server_TakeAttackDamage_Implementation(const float damage)
{
	HealthPoint -= damage;
	OnRep_HealthPoint();

	if (HealthPoint <= 0)
	{
		HealthPoint = 0.f;
		Multicast_Die();
		Destroy();
		//FTimerHandle THDie;
		//GetWorld()->GetTimerManager().SetTimer(THDie, this, &ABaseCharacter::Destroy, 3.0f, false);
	}
}

void ABaseCharacter::Multicast_Die_Implementation()
{
	BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseCharacter::OnRep_HealthPoint()
{
}

void ABaseCharacter::Server_SelectTarget_Implementation()
{
	SelectTarget();
}