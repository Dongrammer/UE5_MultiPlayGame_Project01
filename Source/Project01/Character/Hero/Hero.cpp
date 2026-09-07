#include "Character/Hero/Hero.h"
#include "Helper.h"
#include "Components/CapsuleComponent.h"

// Camera
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
// Input
#include "System/PC_Stage.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Input/InputDataAsset.h"
// Game
#include "System/PJ01_PlayerStateBase.h"
// World
#include "World/WorldManager.h"
#include "World/WorldData.h"
// Widget
#include "Widget/MainHUD.h"
// Attack
#include "Character/HeroAttackComponent.h"
// Equip
#include "EquipComponent.h"
// Item
#include "Item/ItemData.h"


DEFINE_LOG_CATEGORY(HeroLog);

AHero::AHero()
{
	PrimaryActorTick.bCanEverTick = true;
	TopRootComponent->SetCollisionProfileName("Playerd");
	// Create Camera
	CameraArm = Helper::CreateSceneComponent<USpringArmComponent>(this, "Camera Arm", RootComponent);
	CameraArm->TargetArmLength = 400.0f;
	CameraArm->SetRelativeLocation({ 0, 0, 1500 });
	CameraArm->bUsePawnControlRotation = false;
	CameraArm->bInheritYaw = false;
	CameraArm->bInheritPitch = false;
	CameraArm->bInheritRoll = false;

	Camera = Helper::CreateSceneComponent<UCameraComponent>(this, "Camera", CameraArm);
	Camera->SetRelativeRotation(FRotator(-70, 0, 0));

	// Component
	EquipComponent = Helper::CreateActorComponent<UEquipComponent>(this, TEXT("Equip Component"));
	if (!EquipComponent)
	{
		UE_LOG(HeroLog, Warning, TEXT("EquipComponent Was Not Created !!"));
		return;
	}
	AttackComponent = Helper::CreateActorComponent<UHeroAttackComponent>(this, "AttackComponent");
}

void AHero::BeginPlay()
{
	Super::BeginPlay();

	if (IsLocallyControlled()) // Only Local Controller
	{
		const TObjectPtr<APlayerController> PlayerController = Cast<APlayerController>(Controller);
		if (PlayerController)
		{
			PlayerController->bShowMouseCursor = true;

			TObjectPtr<UEnhancedInputLocalPlayerSubsystem> SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

			if (SubSystem) SubSystem->AddMappingContext(KeyMappingContext, 0);

		}
		if (APC_Stage* pc = Cast<APC_Stage>(PlayerController))
		{
			MainHUD = pc->GetMainHUD();

			if (!MainHUD) UE_LOG(HeroLog, Warning, TEXT("MainHUD Is NULL !!"));
		}
	}

	if (HasAuthority())
	{
		// Equip
		if (EquipComponent)
		{
			EquipComponent->DWeaponChanged.AddUFunction(this, "WeaponChanged");
		}
		
	}
	TeamID = 0;
	GetWorld()->GetTimerManager().SetTimer(THPlayerReady, this, &AHero::CheckPlayerReady, 0.1f, true);
}

void AHero::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AHero::CheckPlayerReady()
{
	if (!WorldManager) return;
	if (!AttackComponent) return;
	if (!EquipComponent) return;

	APC_Stage* PCS = Cast<APC_Stage>(GetController());
	if (!PCS) return;
	if (!PCS->GetControllerReady()) return;


	APJ01_PlayerStateBase* PS = GetPlayerState<APJ01_PlayerStateBase>();
	if (!PS) return;

	UE_LOG(LogTemp, Warning, TEXT("CheckPlayer : %s"), *this->GetName());
	PS->Server_SetPlayerReady(true);
	GetWorld()->GetTimerManager().ClearTimer(THPlayerReady);
}

void AHero::WorldManagerSetting(AWorldManager* wm)
{
	Super::WorldManagerSetting(wm);

	if (WorldManager)
	{
		WorldManager->DWorldTimeChanged.AddUFunction(this, "OnWorldTimeChanged");
		WorldManager->AddHero(this);
	}
}

void AHero::OnWorldTimeChanged(int time)
{
	if (!MainHUD) return;
	
	MainHUD->SetWorldRunningTime(time);
}

void AHero::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	TObjectPtr<UEnhancedInputComponent> EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (!EnhancedInputComponent)
	{
		UE_LOG(HeroLog, Warning, TEXT("EnhancedInputComponent Is NULL !!"));
		return;
	}

	MappingInputAsset(EnhancedInputComponent);
}

void AHero::MappingInputAsset(UEnhancedInputComponent* Comp)
{
	if (!InputAsset)
	{
		UE_LOG(HeroLog, Warning, TEXT("InputDataAsset Is NULL !!"));
		return;
	}

	Comp->BindAction(InputAsset->MoveInput, ETriggerEvent::Triggered, this, &AHero::Move);
	Comp->BindAction(InputAsset->JumpAction, ETriggerEvent::Started, this, &AHero::DoJump);
	Comp->BindAction(InputAsset->TurnAction, ETriggerEvent::Started, this, &AHero::DoTurn);
}

void AHero::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (!Controller)
	{
		UE_LOG(HeroLog, Warning, TEXT("PlayerController Is NULL !!"));
		return;
	}

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation = FRotator(0, Rotation.Yaw, 0);

	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.Y);
	AddMovementInput(RightDirection, MovementVector.X);
}

void AHero::DoJump()
{
	Jump();
}

void AHero::DoTurn()
{
	AttackComponent->SetAttackTarget(nullptr);
	SetFrontBack(!bInFront);
	SelectTarget();
}

void AHero::OnRep_InFront()
{
	Super::OnRep_InFront();
	SetActorRotation(bInFront ? FRotator(0, 0, 0) : FRotator(0, 180, 0));
}

void AHero::StartRunning()
{
	Super::StartRunning();

}

void AHero::WeaponChanged(FName WeaponID)
{
	if (!AttackComponent)
	{
		UE_LOG(HeroLog, Warning, TEXT("WeaponChanged Can't Binding !! : WeaponComponent Is NULL !!"));
		return;
	}
	UHeroAttackComponent* HeroAttackComp = Cast<UHeroAttackComponent>(AttackComponent);
	HeroAttackComp->Server_SetCurrentWeapon(WeaponID);
}
