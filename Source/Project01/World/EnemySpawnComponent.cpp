// Fill out your copyright notice in the Description page of Project Settings.


#include "World/EnemySpawnComponent.h"

// Sets default values for this component's properties
UEnemySpawnComponent::UEnemySpawnComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEnemySpawnComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEnemySpawnComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEnemySpawnComponent::InitStatus(FStageEnemyStatusTable* table)
{
	if (!GetOwner() || !GetOwner()->HasAuthority()) return;
	EnemyStatus = table;
}

void UEnemySpawnComponent::StartSpawn()
{
	if (!GetOwner()->HasAuthority()) return;


	GetWorld()->GetTimerManager().SetTimer(SpawnHandle, this, &UEnemySpawnComponent::EnemySpawn, 1.0f, false);
}

void UEnemySpawnComponent::EnemySpawn()
{
}

