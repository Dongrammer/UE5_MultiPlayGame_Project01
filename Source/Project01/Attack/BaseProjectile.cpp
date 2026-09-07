#include "Attack/BaseProjectile.h"
#include "Helper.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Character/BaseCharacter.h"

ABaseProjectile::ABaseProjectile()
{
	PrimaryActorTick.bCanEverTick = true;
	RootComponent = Helper::CreateSceneComponent<USceneComponent>(this, TEXT("Root Component"));
	
	// Collision
	CollisionComponent = Helper::CreateSceneComponent<USphereComponent>(this, TEXT("Collision"), RootComponent);
	CollisionComponent->SetCollisionProfileName("Projectile");
	//CollisionComponent->OnComponentHit.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABaseProjectile::OnProjectileHit);
	
	// Movement
	MovementComponent = Helper::CreateActorComponent<UProjectileMovementComponent>(this, TEXT("Projectile Movement"));
	MovementComponent->bRotationFollowsVelocity = true;
	MovementComponent->bShouldBounce = false;
	MovementComponent->ProjectileGravityScale = 0.f;

	//Mesh
	SMeshComponent = Helper::CreateSceneComponent<UStaticMeshComponent>(this, TEXT("SMesh"), RootComponent);
	SMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

void ABaseProjectile::BeginPlay()
{
	Super::BeginPlay();
}

void ABaseProjectile::InitVelocity(const FVector& ShootDir)
{
	if (!MovementComponent) return;
	MovementComponent->Velocity = ShootDir * MovementComponent->InitialSpeed;
}

void ABaseProjectile::OnProjectileHit(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!HasAuthority()) return;
	if (!OtherActor || OtherActor == GetOwner()) return;
	ApplyAttack(OtherActor, SweepResult);
	Multicast_PlayImpactFX(SweepResult.ImpactPoint);
	Destroy();
}

void ABaseProjectile::ApplyAttack(AActor* OtherActor, const FHitResult& Hit)
{
	ABaseCharacter* HitCharacter = Cast<ABaseCharacter>(OtherActor);
	if (!HitCharacter || !HitCharacter->GetCharacterMovement()) return;

	ABaseCharacter* OwnerC = Cast<ABaseCharacter>(GetOwner());
	if (!OwnerC) return;

	UE_LOG(LogTemp, Warning, TEXT("Owner %d : Hit %d"), OwnerC->GetTeamID(), HitCharacter->GetTeamID());
	// Damage
	if (OwnerC->GetTeamID() == HitCharacter->GetTeamID()) return;
	HitCharacter->Server_TakeAttackDamage(ProjectileData.Damage);

	// Knockback
	FVector HitDir = HitCharacter->GetActorLocation() - GetActorLocation().GetSafeNormal();
	HitDir.Z = 0.5f;
	HitDir.Normalize();
	HitDir = HitCharacter->GetFrontBack() == true ? HitDir * -1 : HitDir * 1;

	FVector LaunchVelocity = HitDir * ProjectileData.KnockbackDamage;
	HitCharacter->AddActorLocalOffset(LaunchVelocity);
	//HitCharacter->LaunchCharacter(LaunchVelocity, true, true);
}

void ABaseProjectile::Multicast_PlayImpactFX_Implementation(FVector_NetQuantize Location)
{
	if (ImpactEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, Location);
	}

	if (ImpactSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), ImpactSound, Location);
	}
}

void ABaseProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

