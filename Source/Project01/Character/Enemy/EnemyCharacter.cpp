#include "Character/Enemy/EnemyCharacter.h"
#include "Helper.h"
// Server
#include "Net/UnrealNetwork.h"
// World
#include "World/WorldManager.h"
// Attack
#include "Character/EnemyAttackComponent.h"
// Widget
#include "Components/WidgetComponent.h"
#include "Widget/EnemyHealthBar.h"

DEFINE_LOG_CATEGORY(EnemyLog);

AEnemyCharacter::AEnemyCharacter()
{
	bReplicates = true;
	AttackComponent = Helper::CreateActorComponent<UEnemyAttackComponent>(this, "AttackComponent");
	//WB_HPBar = Helper::CreateActorComponent<UEnemyHealthBarComponent>(this, TEXT("HealthBar"));
	WB_HPBar = Helper::CreateSceneComponent<UWidgetComponent>(this, TEXT("HealthBar"), RootComponent);
}

void AEnemyCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority()) ChaseHeroTick(DeltaSeconds);
}

void AEnemyCharacter::StartRunning()
{
	Super::StartRunning();

}

void AEnemyCharacter::SetFrontBack(bool front)
{
	Super::SetFrontBack(front);
	SetShouldMove(!bInFront);
}

void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	if (!WB_HPBarClass)
	{
		UE_LOG(EnemyLog, Warning, TEXT("%s : WB_HPBarClass Is NULL !!"), *GetName());
		return;
	}
	WB_HPBar->SetWidgetClass(WB_HPBarClass);
	WB_HPBar->SetWidgetSpace(EWidgetSpace::Screen);
	WB_HPBar->SetRelativeLocation(FVector(0.f, 100.f, 0.f));
	WB_HPBar->SetDrawSize(FVector2D(200.f, 10.f));

	HealthPoint = MaxHealthPoint;
	OnRep_HealthPoint();
	//if (HasAuthority())
		TeamID = 1;
	// temp
	Speed = 300.0f;
}

void AEnemyCharacter::ChaseHeroTick(float Dt)
{
	if (!AttackComponent || !WorldManager || !AttackComponent->GetAttackTarget()) return;

	// Location
	FVector Start = GetActorLocation();
	FVector End = AttackComponent->GetAttackTarget()->GetActorLocation();
	FVector Dir = End - Start;
	if (!Dir.Normalize()) return;
	
	// Chase Speed
	const float WorldSpeed = WorldManager->GetWorldSpeed();
	
	float ChaseSpeed = 0.f;
	if (bInFront)
	{
		ChaseSpeed = WorldSpeed;
	}
	else
	{
		if (!bShouldMove) return;
		ChaseSpeed = Speed - WorldSpeed;
	}

	FVector Delta = Dir * ChaseSpeed * Dt;
	FHitResult Hit;
	AddActorWorldOffset(Delta, true, &Hit);

}

void AEnemyCharacter::OnRep_HealthPoint()
{
	Super::OnRep_HealthPoint();

	if (UEnemyHealthBar* HPB = Cast<UEnemyHealthBar>(WB_HPBar->GetUserWidgetObject()))
	{
		HPB->UpdateHPBar(HealthPoint, MaxHealthPoint);
	}
}
