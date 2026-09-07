#include "Character/Hero/EquipComponent.h"
#include "Net/UnrealNetwork.h"
#include "Hero.h"
#include "Item/ItemSubsystem.h"

DEFINE_LOG_CATEGORY(EquipCompLog);

UEquipComponent::UEquipComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipComponent::BeginPlay()
{
	Super::BeginPlay();

	Owner = Cast<AHero>(GetOwner());
	if (!Owner)
	{
		UE_LOG(EquipCompLog, Warning, TEXT("Owner Is NULL !!"));
		return;
	}
	ItemSubsystem = GetWorld()->GetGameInstance()->GetSubsystem<UItemSubsystem>();
	if (!ItemSubsystem)
	{
		UE_LOG(EquipCompLog, Warning, TEXT("ItemSubsystem Is NULL !!"));
		return;
	}
}


// Called every frame
void UEquipComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipComponent::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UEquipComponent, CurrentWeaponID);
}

void UEquipComponent::EquipWeapon(FName WeaponID)
{
	if (Owner->HasAuthority())
	{
		CurrentWeaponID = WeaponID;
		OnRep_CurrentWeapon();
	}
	else
	{
		Server_EquipWeapon(WeaponID);
	}

}

void UEquipComponent::Server_EquipWeapon_Implementation(FName WeaponID)
{
	EquipWeapon(WeaponID);
}

void UEquipComponent::OnRep_CurrentWeapon()
{
	if (!ItemSubsystem) return;

	if (DWeaponChanged.IsBound()) DWeaponChanged.Broadcast(CurrentWeaponID);


}

