#include "Character/HeroAttackComponent.h"
#include "Animation/Hero_AnimDataAsset.h"
#include "Character/BaseCharacter.h"
#include "Character/Hero/Hero.h"
// Net
#include "Net/UnrealNetwork.h"
// Targeting
#include "Enemy/EnemyCharacter.h"
#include "Kismet/KismetSystemLibrary.h"
// Projectile
#include "Kismet/GameplayStatics.h"
#include "Attack/BaseProjectile.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
// Item
#include "Item/ItemSubsystem.h"
// Debug
#include "System/DebugSubsystem.h"
#include "DrawDebugHelpers.h"

void UHeroAttackComponent::BeginPlay()
{
	Super::BeginPlay();

	ItemSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (!ItemSubsystem)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("ItemSubsystem Is NULL !!"));
		return;
	}

	AttackRange = 1200.0f;
}

void UHeroAttackComponent::Multicast_PlayAttackMontage_Implementation()
{
	if (!HeroAnim)
	{
		UE_LOG(LogTemp, Warning, TEXT("HeroAnim Is NULL !!"));
		return;
	}
	const EWeaponType type = CurrentWeaponData.WeaponType;
	UAnimMontage* montage = HeroAnim->WeaponAnims.FindRef(type);
	if (!montage)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s : AnimMontage Is NULL !!"), *OwnerCharacter.GetName());
		return;
	}
	bCanNextAttack = false;
	OwnerCharacter->PlayAnimMontage(montage, AttackSpeed);
}

void UHeroAttackComponent::SelectAttackTarget()
{
	Super::SelectAttackTarget();

	if (!OwnerCharacter || !OwnerCharacter->HasAuthority()) return;

	TArray<AActor*> Overlaps;
	const TArray<TEnumAsByte<EObjectTypeQuery>> ObjTypes = { UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn) };
	UKismetSystemLibrary::SphereOverlapActors(this, OwnerCharacter->GetActorLocation(), AttackRange, ObjTypes, AEnemyCharacter::StaticClass(), {OwnerCharacter}, Overlaps);

	if (Overlaps.Num() == 0) return;

	FVector Fwd = FVector::ForwardVector; // 항상 위쪽(뒤돌았으면 뒤쪽)을 향하여
	if (!OwnerCharacter->GetFrontBack()) Fwd *= -1.f;
	AActor* target = nullptr;
	float targetdis = TNumericLimits<float>::Max();

	for (AActor* actor : Overlaps)
	{
		if (!actor) continue;
		ABaseCharacter* hitc = Cast<ABaseCharacter>(actor);
		if (!hitc) continue;
		if (hitc->GetTeamID() == 0) continue;

		FVector To = actor->GetActorLocation() - OwnerCharacter->GetActorLocation();
		float Dot = FVector::DotProduct(Fwd, To);
		Dot = FMath::Clamp(Dot, -1.f, 1.f);
		const float Angle = FMath::RadiansToDegrees(acosf(Dot));
		if (Angle > HalfFOV) continue;

		const float Dis = FVector::DistSquared2D(actor->GetActorLocation(), OwnerCharacter->GetActorLocation());
		if (Dis < targetdis)
		{
			targetdis = Dis;
			target = actor;
		}
	}

	if (!target) return;

	TObjectPtr<ABaseCharacter> c = Cast<ABaseCharacter>(target);
	if (!c)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("Target Casting Fail !!"));
		return;
	}
	if (AttackTarget != target)
	{
		AttackTarget = c;
		OnRep_AttackTarget();
	}

	// Debug
	if (GetWorld()->GetGameInstance()->GetSubsystem<UDebugSubsystem>()->bAttackDebugMode)
	{
		FVector Origin = OwnerCharacter->GetActorLocation();
		FVector Fwdv = FVector::ForwardVector;
		if (!OwnerCharacter->GetFrontBack()) Fwdv *= -1.f;
		Fwdv.Normalize();
		float HalfRad = FMath::DegreesToRadians(HalfFOV);
		DrawDebugCone(GetWorld(), Origin, Fwdv, AttackRange, HalfRad, HalfRad, 30, FColor::Blue, false, 1.f);
		if (target)
		{
			DrawDebugSphere(GetWorld(), target->GetActorLocation(), 50.f, 12, FColor::Red, false, 0.5f);
		}
	}
}

void UHeroAttackComponent::DoAttack()
{
	Super::DoAttack();

	if (!OwnerCharacter || !AttackTarget) return;
	if (OwnerCharacter->HasAuthority())
	{
		Multicast_PlayAttackMontage();
	}
	else 
	{
		Server_DoAttack();
	}
}

void UHeroAttackComponent::Server_DoAttack_Implementation()
{
	DoAttack();
}

void UHeroAttackComponent::Server_SetCurrentWeapon_Implementation(FName WeaponID)
{
	CurrentWeaponID = WeaponID;
	if (!ItemSubsystem)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("ItemSubsystem Is NULL !!"));
		return;
	}

	CurrentWeaponData = ItemSubsystem->GetWeaponData(WeaponID);
}

void UHeroAttackComponent::FireProjectileNotify()
{
	Super::FireProjectileNotify();

	if (!OwnerCharacter->HasAuthority()) return;
	if (CurrentWeaponData.ItemType != EItemType::E_Weapon)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("WeaponType Is Not Currect !!"));
		return;
	}
	TSubclassOf<ABaseProjectile> ProjectileClass = CurrentWeaponData.ProjectileClass;
	UParticleSystem* FireFX = CurrentWeaponData.FireEffect;
	USoundBase* FireSound = CurrentWeaponData.FireSound;

	if (!ProjectileClass)
	{
		UE_LOG(AttackCompLog, Warning, TEXT("ProjectileClass Is NULL !!"));
		return;
	}

	FVector MuzzleLoc = OwnerCharacter->GetActorLocation() + OwnerCharacter->GetActorForwardVector() * 100.f;
	FRotator MuzzleRot = OwnerCharacter->GetActorRotation();

	FActorSpawnParameters SpawnParam;
	SpawnParam.Owner = OwnerCharacter;
	SpawnParam.Instigator = OwnerCharacter;
	TObjectPtr<ABaseProjectile> projectile = GetWorld()->SpawnActor<ABaseProjectile>(ProjectileClass, MuzzleLoc, MuzzleRot, SpawnParam);
	if (projectile)
	{
		projectile->InitVelocity(MuzzleRot.Vector());
	}

	if (FireFX) UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), FireFX, MuzzleLoc, MuzzleRot);
	if (FireSound) UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, MuzzleLoc);
}

void UHeroAttackComponent::EndAttackNotify()
{
	Super::EndAttackNotify();

}

void UHeroAttackComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UHeroAttackComponent, CurrentWeaponID);
	DOREPLIFETIME(UHeroAttackComponent, CurrentWeaponData);
}