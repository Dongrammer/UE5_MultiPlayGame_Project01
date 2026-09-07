#include "World/EnemySpawnArea.h"
#include "Components/BoxComponent.h"
#include "Helper.h"

AEnemySpawnArea::AEnemySpawnArea()
{
	PrimaryActorTick.bCanEverTick = true;

	BoxComponent = Helper::CreateSceneComponent<UBoxComponent>(this, TEXT("BoxArea"), RootComponent);
	BoxComponent->SetBoxExtent(FVector(200, 200, 10));
}

void AEnemySpawnArea::BeginPlay()
{
	Super::BeginPlay();
	
}

void AEnemySpawnArea::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemySpawnArea::SetBoxSize(FVector size)
{
	BoxComponent->SetBoxExtent(size);
}

FVector AEnemySpawnArea::GetRandomPoint() const
{
	FVector Extent = BoxComponent->GetScaledBoxExtent();
	FVector Origin = BoxComponent->GetComponentLocation();

	float RandX = FMath::FRandRange(-Extent.X, Extent.X);
	float RandY = FMath::FRandRange(-Extent.Y, Extent.Y);

	return FVector(Origin.X + RandX, Origin.Y + RandY, Origin.Z + 50.0f);
}

