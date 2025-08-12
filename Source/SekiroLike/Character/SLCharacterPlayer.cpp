// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterPlayer.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SLCharacterControlData.h"
#include "Camera/SLFreeCamera.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/SLAnimInstance.h"
#include "UI/SLHUDWidget.h"
#include "CharacterStat/SLCharacterStatComponent.h"
#include "Interface/SLGameInterface.h"
#include "GameFramework/GameModeBase.h"
#include "GameData/SLGameSingleton.h"
#include "Components/CapsuleComponent.h"
#include "Physics/SLCollision.h"
#include "Engine/DamageEvents.h"
#include "Curve/SLCurveManager.h"
#include "Particles/ParticleSystemComponent.h"
#include "Kismet/GameplayStatics.h"
#include "TargetSystem/SLTargetSystemComponent.h"



ASLCharacterPlayer::ASLCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 8.5f;

	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;


	//여기에 이름을 붙일 때 소켓으로 붙임. Spring Arm 끝에 붙는 것임.

	// Input Action

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionJumpRef(TEXT("/Game/SekiroLike/Input/Actions/IA_Jump.IA_Jump"));
	if (nullptr != InputActionJumpRef.Object)
	{
		JumpAction = InputActionJumpRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputChangeActionControlRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_ChangeControl.IA_ChangeControl'"));
	if (nullptr != InputChangeActionControlRef.Object)
	{
		ChangeControlAction = InputChangeActionControlRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_ShoulderMove.IA_ShoulderMove'"));
	if (nullptr != InputActionShoulderMoveRef.Object)
	{
		ShoulderMoveAction = InputActionShoulderMoveRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShoulderLookRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_ShoulderLook.IA_ShoulderLook'"));
	if (nullptr != InputActionShoulderLookRef.Object)
	{
		ShoulderLookAction = InputActionShoulderLookRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionQuaterMoveRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_QuaterMove.IA_QuaterMove'"));
	if (nullptr != InputActionQuaterMoveRef.Object)
	{
		QuaterMoveAction = InputActionQuaterMoveRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionToggleFreeCamRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_ToggleFreeCam.IA_ToggleFreeCam'"));
	if (nullptr != InputActionToggleFreeCamRef.Object)
	{
		ToggleFreeCamAction = InputActionToggleFreeCamRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_Attack.IA_Attack'"));
	if (nullptr != InputActionAttackRef.Object)
	{
		AttackAction = InputActionAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionSprintStartRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_Sprint.IA_Sprint'"));
	if (nullptr != InputActionSprintStartRef.Object)
	{
		SprintAction = InputActionSprintStartRef.Object;
	}
	
	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionStealthWalkRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_Stealth.IA_Stealth'"));
	if (nullptr != InputActionStealthWalkRef.Object)
	{
		StealthWalkAction = InputActionStealthWalkRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionRushAttackRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_RushAttack.IA_RushAttack'"));
	if (nullptr != InputActionRushAttackRef.Object)
	{
		RushAttackAction = InputActionRushAttackRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionTargetingThrowingKnifeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_TargetingThrowingKnife.IA_TargetingThrowingKnife'"));
	if (nullptr != InputActionTargetingThrowingKnifeRef.Object)
	{
		TargetingThrowingKnifeAction = InputActionTargetingThrowingKnifeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionShadowStrikeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_ShadowStrike.IA_ShadowStrike'"));
	if (nullptr != InputActionShadowStrikeRef.Object)
	{
		ShadowStrikeAction = InputActionShadowStrikeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionEvadeRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_Evade.IA_Evade'"));
	if (nullptr != InputActionEvadeRef.Object)
	{
		EvadeAction = InputActionEvadeRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UInputAction> InputActionLockOnRef(TEXT("/Script/EnhancedInput.InputAction'/Game/SekiroLike/Input/Actions/IA_LockOn.IA_LockOn'"));
	if (nullptr != InputActionLockOnRef.Object)
	{
		LockOnAction = InputActionLockOnRef.Object;
	}

	CurrentCharacterControlType = ECharacterControlType::Shoulder;


	// Anim Montage

	static ConstructorHelpers::FObjectFinder<UAnimMontage> RushAttackMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_RushAttack.AM_RushAttack'"));
	if (RushAttackMontageRef.Object)
	{
		RushAttackMontage = RushAttackMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ThrowingKnifeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_ThrowingKnife.AM_ThrowingKnife'"));
	if (ThrowingKnifeMontageRef.Object)
	{
		ThrowingKnifeMontage = ThrowingKnifeMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CancelThrowingKnifeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_CancelThrowingKnife.AM_CancelThrowingKnife'"));
	if (CancelThrowingKnifeMontageRef.Object)
	{
		CancelThrowingKnifeMontage = CancelThrowingKnifeMontageRef.Object;
	}	

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ShadowStrikeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_ShadowStrike.AM_ShadowStrike'"));
	if (ShadowStrikeMontageRef.Object)
	{
		ShadowStrikeMontage = ShadowStrikeMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> EvadeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_Evade.AM_Evade'"));
	if (EvadeMontageRef.Object)
	{
		EvadeMontage = EvadeMontageRef.Object;
	}	

	/*Effect*/
	StealthEnterEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StealthEnterEffect"));
	StealthEnterEffect->SetupAttachment(GetMesh(), TEXT("pelvis"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> StealthEnterEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/ShadowPlane/FX/ShadowPlaneEnter2.ShadowPlaneEnter2'"));
	if (StealthEnterEffectRef.Object)
	{
		StealthEnterEffect->SetTemplate(StealthEnterEffectRef.Object);
		StealthEnterEffect->bAutoActivate = false;
	}

	StealthExitEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("StealthExitEffect"));
	StealthExitEffect->SetupAttachment(GetMesh(), TEXT("pelvis"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> StealthExitEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/ShadowPlane/FX/ShadowPlaneExit2.ShadowPlaneExit2'"));
	if (StealthExitEffectRef.Object)
	{
		StealthExitEffect->SetTemplate(StealthExitEffectRef.Object);
		StealthExitEffect->bAutoActivate = false;
	}

	ComboEffect_L = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ComboEffect_L"));
	ComboEffect_R = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ComboEffect_R"));
	ComboEffect_L->SetupAttachment(GetMesh(), TEXT("FX_Trail_L_01"));
	ComboEffect_R->SetupAttachment(GetMesh(), TEXT("FX_Trail_R01")); 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ComboEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/ShadowPlane/FX/P_ShadowPlane_Bonus_Damage.P_ShadowPlane_Bonus_Damage'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Shadow_Bonus_Damage_Rogue.P_Shadow_Bonus_Damage_Rogue'
	if (ComboEffectRef.Object)
	{
		ComboEffect_L->SetTemplate(ComboEffectRef.Object);
		ComboEffect_R->SetTemplate(ComboEffectRef.Object);
		ComboEffect_L->bAutoActivate = false;
		ComboEffect_R->bAutoActivate = false;
	}

	BurstEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BurstEffect"));
	BurstEffect->SetupAttachment(GetMesh(), TEXT("FX_Thruster_Center"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BurstEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/Ultimate/FX/P_BackJets_Trail_Burst_Ult.P_BackJets_Trail_Burst_Ult'"));
	if (BurstEffectRef.Object)
	{
		BurstEffect->SetTemplate(BurstEffectRef.Object);
		BurstEffect->bAutoActivate = false;
	}

	RushWeaponEffect_L = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RushWeaponEffectL"));
	RushWeaponEffect_R = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RushWeaponEffectR"));
	RushWeaponEffect_L->SetupAttachment(GetMesh(), TEXT("FX_Trail_L_01")); //FX_Trail_L_01, sword_tip_l, sword_base_l, Melee_Impact_L, sword_handle_l, weapon_l, sword_root_l
	RushWeaponEffect_R->SetupAttachment(GetMesh(), TEXT("FX_Trail_R01")); //FX_Trail_R01,  sword_tip_r, sword_base_r, Melee_Impact_R, sword_handle_r, weapon_r, sword_root_r
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RushWeaponEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/Ultimate/FX/P_Ultimate_Weapon_Charge.P_Ultimate_Weapon_Charge'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Ultimate_Weapon_Charge_Rogue.P_Ultimate_Weapon_Charge_Rogue'
	if (RushWeaponEffectRef.Object)
	{
		RushWeaponEffect_L->SetTemplate(RushWeaponEffectRef.Object);
		RushWeaponEffect_R->SetTemplate(RushWeaponEffectRef.Object);
		RushWeaponEffect_L->bAutoActivate = false;
		RushWeaponEffect_R->bAutoActivate = false;
	}

	RushWindEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("RushWindEffect"));
	RushWindEffect->SetupAttachment(GetMesh(), TEXT("Aim_Target")); //pelvis, DashStartPoint, SlashARcs, Status, 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> RushWindEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/Ultimate/FX/P_Ultimate_Rush_Wind.P_Ultimate_Rush_Wind'"));
	if (RushWindEffectRef.Object)
	{
		RushWindEffect->SetTemplate(RushWindEffectRef.Object);
		RushWindEffect->bAutoActivate = false;
	}

	TargetingEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("TargetingEffect"));
	TargetingEffect->SetupAttachment(GetMesh(), TEXT("dagger_a_r")); 
	static ConstructorHelpers::FObjectFinder<UParticleSystem> TargetingEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/DaggerThrow/FX/P_Kallari_Dagger_Targeting.P_Kallari_Dagger_Targeting'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Dagger_Targeting_Rogue.P_Dagger_Targeting_Rogue'
	if (TargetingEffectRef.Object)
	{
		TargetingEffect->SetTemplate(TargetingEffectRef.Object);
		TargetingEffect->bAutoActivate = false;
	}

	DodgeEffect1 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DodgeEffect1"));
	DodgeEffect1->SetupAttachment(GetMesh(), TEXT("FX_Thruster_Center"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DodgeEffect1Ref(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/Dodge/FX/P_BackJets_Trail_Burst.P_BackJets_Trail_Burst'"));
	//
	if (DodgeEffect1Ref.Object)
	{
		DodgeEffect1->SetTemplate(DodgeEffect1Ref.Object);
		DodgeEffect1->bAutoActivate = false;
	}
	DodgeEffect2 = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("DodgeEffect2"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> DodgeEffect2Ref(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/ShadowPlane/FX/P_ShadowPlane_Warmup.P_ShadowPlane_Warmup'"));
	//
	if (DodgeEffect2Ref.Object)
	{
		DodgeEffect2->SetTemplate(DodgeEffect2Ref.Object);
		DodgeEffect2->bAutoActivate = false;
	}

	ShadowStrikeWeaponEffect_L = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShadowStrikeWeaponEffect_L"));
	ShadowStrikeWeaponEffect_R = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShadowStrikeWeaponEffect_R"));
	ShadowStrikeWeaponEffect_L->SetupAttachment(GetMesh(), TEXT("FX_Trail_L_01"));
	ShadowStrikeWeaponEffect_R->SetupAttachment(GetMesh(), TEXT("FX_Trail_R01"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ShadowStrikeWeaponEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Ultimate_Weapon_Charge_Rogue.P_Ultimate_Weapon_Charge_Rogue'"));
	if (ShadowStrikeWeaponEffectRef.Object)
	{
		ShadowStrikeWeaponEffect_L->SetTemplate(ShadowStrikeWeaponEffectRef.Object);
		ShadowStrikeWeaponEffect_R->SetTemplate(ShadowStrikeWeaponEffectRef.Object);
		ShadowStrikeWeaponEffect_L->bAutoActivate = false;
		ShadowStrikeWeaponEffect_R->bAutoActivate = false;

	}

	ShadowStrikeBodyEffect = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShadowStrikeBodyEffect"));
	ShadowStrikeBodyEffect->SetupAttachment(GetMesh(), TEXT("FX_Thruster_Center"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> ShadowStrikeBodyEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_BackJets_Trail_Rogue.P_BackJets_Trail_Rogue'"));
	if (ShadowStrikeBodyEffectRef.Object)
	{
		ShadowStrikeBodyEffect->SetTemplate(ShadowStrikeBodyEffectRef.Object);
		ShadowStrikeBodyEffect->bAutoActivate = false;
	}
	
	BloodEffect_LH = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BloodEffect_LH"));
	BloodEffect_RH = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BloodEffect_RH"));
	BloodEffect_LF = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BloodEffect_LF"));
	BloodEffect_RF = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("BloodEffect_RF"));
	BloodEffect_LH->SetupAttachment(GetMesh(), TEXT("Muzzle_02")); //hand_l
	BloodEffect_RH->SetupAttachment(GetMesh(), TEXT("Muzzle_01")); //hand_r
	BloodEffect_LF->SetupAttachment(GetMesh(), TEXT("Foot_L"));
	BloodEffect_RF->SetupAttachment(GetMesh(), TEXT("Foot_R"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> BloodEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_Ultimate_Warmup_Limbs_Rogue.P_Ultimate_Warmup_Limbs_Rogue'"));
	///Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Skins/Rogue/P_ShadowPlane_Exit_Limbs_Rogue.P_ShadowPlane_Exit_Limbs_Rogue'
	//
	if (BloodEffectRef.Object)
	{
		BloodEffect_LH->SetTemplate(BloodEffectRef.Object);
		BloodEffect_RH->SetTemplate(BloodEffectRef.Object);
		BloodEffect_LF->SetTemplate(BloodEffectRef.Object);
		BloodEffect_RF->SetTemplate(BloodEffectRef.Object);
		BloodEffect_LH->bAutoActivate = false;
		BloodEffect_RH->bAutoActivate = false;
		BloodEffect_LF->bAutoActivate = false;
		BloodEffect_RF->bAutoActivate = false;
	}

	MpEffect_LH = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MpEffect_LH"));
	MpEffect_RH = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MpEffect_RH"));
	MpEffect_LF = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MpEffect_LF"));
	MpEffect_RF = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("MpEffect_RF"));
	MpEffect_LH->SetupAttachment(GetMesh(), TEXT("Muzzle_02"));
	MpEffect_RH->SetupAttachment(GetMesh(), TEXT("Muzzle_01"));
	MpEffect_LF->SetupAttachment(GetMesh(), TEXT("Foot_L"));
	MpEffect_RF->SetupAttachment(GetMesh(), TEXT("Foot_R"));
	static ConstructorHelpers::FObjectFinder<UParticleSystem> MpEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/ParagonKallari/FX/Particles/Kallari/Abilities/ShadowPlane/FX/P_ShadowPlane_Exit_Limbs.P_ShadowPlane_Exit_Limbs'"));
	if (MpEffectRef.Object)
	{
		MpEffect_LH->SetTemplate(MpEffectRef.Object);
		MpEffect_RH->SetTemplate(MpEffectRef.Object);
		MpEffect_LF->SetTemplate(MpEffectRef.Object);
		MpEffect_RF->SetTemplate(MpEffectRef.Object);
		MpEffect_LH->bAutoActivate = false;
		MpEffect_RH->bAutoActivate = false;
		MpEffect_LF->bAutoActivate = false;
		MpEffect_RF->bAutoActivate = false;
	}

	/*Components*/
	TargetSystem = CreateDefaultSubobject<USLTargetSystemComponent>(TEXT("TargetSystem"));
	TargetSystem->OnTargetChanged.AddUObject(this, &ASLCharacterPlayer::UpdateTarget);
}

void ASLCharacterPlayer::BeginPlay()
{
	Super::BeginPlay();
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		EnableInput(PlayerController);
	}

	SetCharacterControl(CurrentCharacterControlType);

	if (!FreeCameraPawn)
	{
		FActorSpawnParameters Params;
		Params.Owner = this;
		FreeCameraPawn = GetWorld()->SpawnActor<ASLFreeCamera>(ASLFreeCamera::StaticClass(), GetActorLocation(), GetControlRotation(), Params);
		FreeCameraPawn->SetOwnerRef(this);
	}

	if (FreeCameraPawn)
	{
		ASLCharacterPlayer* OwnerCharacter = Cast<ASLCharacterPlayer>(FreeCameraPawn->GetOwner());
		FString OwnerName = OwnerCharacter ? OwnerCharacter->GetName() : TEXT("None");
	}
}

void ASLCharacterPlayer::SetDeath()
{
	Super::SetDeath();

	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (PlayerController)
	{
		DisableInput(PlayerController);

		ISLGameInterface* SLGameMode = Cast<ISLGameInterface>(GetWorld()->GetAuthGameMode());
		if (SLGameMode)
		{
			SLGameMode->OnPlayerDead();
		}
	}
}

void ASLCharacterPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	LockOnTick(DeltaTime);
	SkillTick(DeltaTime);
}

void ASLCharacterPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent);

	//Jumping
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ASLCharacterPlayer::Jump);
	EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);
	
	//Change Control
	EnhancedInputComponent->BindAction(ChangeControlAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::ChangeCharacterControl);

	//Shoulder Moving
	EnhancedInputComponent->BindAction(ShoulderMoveAction, ETriggerEvent::Triggered, this, &ASLCharacterPlayer::ShoulderMove);
	
	//Shoulder Looking
	EnhancedInputComponent->BindAction(ShoulderLookAction, ETriggerEvent::Triggered, this, &ASLCharacterPlayer::ShoulderLook);

	//Quater Moving
	EnhancedInputComponent->BindAction(QuaterMoveAction, ETriggerEvent::Triggered, this, &ASLCharacterPlayer::QuaterMove);

	//FreeCam Toggle
	EnhancedInputComponent->BindAction(ToggleFreeCamAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::ToggleFreeCamera);

	//Attack
	EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::Attack);
	
	//Movement
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::StartSprint);
	EnhancedInputComponent->BindAction(SprintAction, ETriggerEvent::Completed, this, &ASLCharacterPlayer::EndSprint);
	
	//Stealth
	EnhancedInputComponent->BindAction(StealthWalkAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::ToggleStealthMode);
	
	//RushAttack
	EnhancedInputComponent->BindAction(RushAttackAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::RushAttack);
	
	//ThrowingKnife
	EnhancedInputComponent->BindAction(TargetingThrowingKnifeAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::TargetingThrowingKnife);
	EnhancedInputComponent->BindAction(TargetingThrowingKnifeAction, ETriggerEvent::Completed, this, &ASLCharacterPlayer::CancelThrowingKnife);

	//ShadowStrike
	EnhancedInputComponent->BindAction(ShadowStrikeAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::ShadowStrike);

	//Evade
	EnhancedInputComponent->BindAction(EvadeAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::Evade);

	//Evade
	EnhancedInputComponent->BindAction(LockOnAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::ToggleLockOn);
}

void ASLCharacterPlayer::ChangeCharacterControl()
{
	if (CurrentCharacterControlType == ECharacterControlType::Quater)
	{
		SetCharacterControl(ECharacterControlType::Shoulder);
	}
	else if (CurrentCharacterControlType == ECharacterControlType::Shoulder)
	{
		SetCharacterControl(ECharacterControlType::Quater);
	}
}

void ASLCharacterPlayer::SetCharacterControl(ECharacterControlType NewCharacterControlType)
{
	USLCharacterControlData* NewCharacterControl = CharacterControlManager[NewCharacterControlType];
	check(NewCharacterControl);
	
	SetCharacterControlData(NewCharacterControl);
	
	APlayerController* PlayerController = CastChecked<APlayerController>(GetController());
	
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->ClearAllMappings();
		UInputMappingContext* NewMappingContext = NewCharacterControl->InputMappingContext;
		if (NewMappingContext)
		{
			Subsystem->AddMappingContext(NewMappingContext, 0);
		}
	}
	
	CurrentCharacterControlType = NewCharacterControlType;
}

void ASLCharacterPlayer::SetCharacterControlData(const USLCharacterControlData* CharacterControlData)
{
	Super::SetCharacterControlData(CharacterControlData);
	
	CameraBoom->TargetArmLength = CharacterControlData->TargetArmLength;
	CameraBoom->SetRelativeRotation(CharacterControlData->RelativeRotation);
	CameraBoom->bUsePawnControlRotation = CharacterControlData->bUsePawnControlRotation;
	CameraBoom->bInheritPitch = CharacterControlData->bInheritPitch;
	CameraBoom->bInheritYaw = CharacterControlData->bInheritYaw;
	CameraBoom->bInheritRoll = CharacterControlData->bInheritRoll;
	CameraBoom->bDoCollisionTest = CharacterControlData->bDoCollisionTest;
}

void ASLCharacterPlayer::ToggleFreeCamera(const FInputActionValue& Value)
{	
	if (!bIsUsingFreeCamera)
	{
		// FreeCameraPawn 스폰 또는 이미 스폰된 인스턴스 참고
		if (!FreeCameraPawn)
		{
			FActorSpawnParameters Params;
			Params.Owner = this;
			FreeCameraPawn->SetOwnerRef(this);
			FreeCameraPawn = GetWorld()->SpawnActor<ASLFreeCamera>(ASLFreeCamera::StaticClass(), GetActorLocation(), GetControlRotation(), Params);
		}
		
		APlayerController* PC = CastChecked<APlayerController>(GetController());
		UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PC->GetLocalPlayer());

		// 입력 맵핑 교체
		UInputMappingContext* NewMappingContext = FreeCameraPawn->GetInputMappingContext();
		if (NewMappingContext)
		{
			Subsystem->ClearAllMappings();
			Subsystem->AddMappingContext(NewMappingContext, 0);

			// Possess
			PC->Possess(Cast<APawn>(FreeCameraPawn));
			bIsUsingFreeCamera = true;
		}
	}
	else
	{
		SetCharacterControl(CurrentCharacterControlType);  // 카메라붐 설정 등 복원
		bIsUsingFreeCamera = false;
	}
}

void ASLCharacterPlayer::Jump()
{
	if (bIsStealth)
		return;

	Super::Jump();
}

void ASLCharacterPlayer::ShoulderMove(const FInputActionValue& Value)
{
	if (bIsActing && !bIsTargeting)
		return;

	FVector2D MovementVector = Value.Get<FVector2D>();

	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, MovementVector.X);
	AddMovementInput(RightDirection, MovementVector.Y);
}

void ASLCharacterPlayer::ShoulderLook(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();
	if (bIsLockOn)
	{
		TargetSystem->ChangeTargetForMouse(LookAxisVector);
		return;
	}

	AddControllerYawInput(LookAxisVector.X);
	AddControllerPitchInput(LookAxisVector.Y);

}

void ASLCharacterPlayer::QuaterMove(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	float MovementVectorSize = 1.0f;
	float LengthSquared = MovementVector.SquaredLength();
	
	if (LengthSquared <= 1.0f)
	{
		MovementVectorSize = FMath::Sqrt(LengthSquared);
	}
	MovementVector.Normalize();

	FVector MoveDirection = FVector(MovementVector.X, MovementVector.Y, 0.0f);
	GetController()->SetControlRotation(FRotationMatrix::MakeFromX(MoveDirection).Rotator());
	AddMovementInput(MoveDirection, MovementVectorSize);
}

void ASLCharacterPlayer::Attack()
{
	if (bIsTargeting)
	{
		ThrowKnife();
		return;
	}
	
	ProcessComboCommand();
}

void ASLCharacterPlayer::SetupHUDWidget(USLHUDWidget* InHUDWidget)
{
	if (InHUDWidget)
	{
		InHUDWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		InHUDWidget->UpdateBoostStat(Stat->GetBoostStat());
		InHUDWidget->UpdateHpBar(Stat->GetCurrentHp());
		InHUDWidget->UpdateMpBar(Stat->GetCurrentMp());

		Stat->OnStatChanged.AddUObject(InHUDWidget, &USLHUDWidget::UpdateStat);
		Stat->OnBoostStatChanged.AddUObject(InHUDWidget, &USLHUDWidget::UpdateBoostStat);
		Stat->OnHpChanged.AddUObject(InHUDWidget, &USLHUDWidget::UpdateHpBar);
		Stat->OnMpChanged.AddUObject(InHUDWidget, &USLHUDWidget::UpdateMpBar);
		Stat->OnExpChanged.AddUObject(InHUDWidget, &USLHUDWidget::UpdateExpBar);

	}
}

void ASLCharacterPlayer::StartSprint()
{
	if (Stat->GetCurrentMp() < 3.f)
		return;

	if (bIsStealth)
	{
		OffStealth();
	}
		
	UE_LOG(LogTemp, Log, TEXT("StartSprint"));
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
}

void ASLCharacterPlayer::EndSprint()
{
	if (bIsStealth) return;

	float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	if (CurrentSpeed <= NormalSpeed)
		return;

	UE_LOG(LogTemp, Log, TEXT("EndSprint"));

	bSmoothing = true;
	TargetSpeed = NormalSpeed;
}

void ASLCharacterPlayer::Dodge(const FVector DodgeDir, const float DodgeSpeed, const float DodgeTime, const ECurveType CurveType)
{
	bIsDodge = true;
	DodgeDirection = DodgeDir * DodgeSpeed;
	DodgeTimeTotal = DodgeTime;
	DodgeTimeLeft = DodgeTimeTotal;
	CurrentCurveType = CurveType;
	GetCharacterMovement()->StopMovementImmediately();
}

void ASLCharacterPlayer::DodgeTick(float DeltaTime)
{
	if (!bIsDodge)
		return;

	UCurveFloat* Curve = ASLCurveManager::GetInstance(GetWorld())->GetCurve(CurrentCurveType);
	if (Curve)
	{	
		float Ratio = 1.f - (DodgeTimeLeft / DodgeTimeTotal);
		float CurveResult = Curve->GetFloatValue(Ratio);
		AddActorWorldOffset(DodgeDirection * CurveResult * DeltaTime, true);
		//UE_LOG(LogTemp, Log, TEXT("CurveResult : %f"), CurveResult);
	}

	DodgeTimeLeft -= DeltaTime;
	if(DodgeTimeLeft <= 0.f)
	{
		bIsDodge = false;
	}
}

void ASLCharacterPlayer::ToggleStealthMode()
{
	if (bIsActing) return;

	if (bIsStealth)
	{
		OffStealth();
	}
	else
	{
		OnStealth();	
	}
}

void ASLCharacterPlayer::OnStealth()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (Cast<USLAnimInstance>(AnimInstance)->GetIsSprinting())
	{
		EndSprint();
		bSmoothing = false;
	}

	bIsStealth = true;
	if (AnimInstance)
	{
		Cast<USLAnimInstance>(AnimInstance)->SetStealthMode(true);
	}

	GetCharacterMovement()->MaxWalkSpeed = StealthWalkSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	{
		GetMesh()->SetMaterial(i, TransparencyMaterials[i]);
	}
	StealthEnterEffect->ActivateSystem();
}

void ASLCharacterPlayer::OffStealth()
{
	bIsStealth = false;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		Cast<USLAnimInstance>(AnimInstance)->SetStealthMode(false);
	}
	
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.f;

	for (int32 i = 0; i < GetMesh()->GetNumMaterials(); ++i)
	{
		GetMesh()->SetMaterial(i, BasicMaterials[i]);
	}
	StealthExitEffect->ActivateSystem();
}

bool ASLCharacterPlayer::CheckMP(float Mp)
{
	float CurrentMP = Stat->GetCurrentMp();

	if (CurrentMP < Mp)
		return false;

	Stat->ApplyMpConsumption(Mp);
	return true;
}

void ASLCharacterPlayer::SkillTick(float DeltaTime)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (Cast<USLAnimInstance>(AnimInstance)->GetIsSprinting())
		{
			if (!CheckMP(3.f * DeltaTime))
			{
				GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
			}
		}
	}

	if (bSmoothing)
	{
		float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;

		float Epsilon = 2.f;
		if (FMath::Abs(TargetSpeed - CurrentSpeed) < Epsilon)
		{
			bSmoothing = false;
			GetCharacterMovement()->MaxWalkSpeed = TargetSpeed;
			UE_LOG(LogTemp, Log, TEXT("Smoothing END"));
		}
		else
		{
			float InterpSpeed = 25.f;
			float NewSpeed = FMath::FInterpTo(CurrentSpeed, TargetSpeed, DeltaTime, InterpSpeed);
			GetCharacterMovement()->MaxWalkSpeed = NewSpeed;
		}
	}

	DodgeTick(DeltaTime);
}

void ASLCharacterPlayer::ComboActionBegin()
{
	if (bIsActing) return;
	bIsActing = true;

	if (bIsStealth)
	{
		OffStealth();
	}

	Super::ComboActionBegin();
	ComboEffect_L->ActivateSystem();
	ComboEffect_R->ActivateSystem();
	//MpEffect_LH->ActivateSystem();
	//MpEffect_RH->ActivateSystem();
	//MpEffect_LF->ActivateSystem();
	//MpEffect_RF->ActivateSystem();
}

void ASLCharacterPlayer::NotifyComboActionEnd()
{
	ComboEffect_L->DeactivateSystem();
	ComboEffect_R->DeactivateSystem();

	if (bIsCanceling)
		return;

	Super::NotifyComboActionEnd();
	bIsActing = false;
}


void ASLCharacterPlayer::RushAttack()
{
	if (bIsActing) return;

	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("RushAttack")));
	if (!CheckMP(CharacterSkillData.Mp)) return;

	bIsActing = true;

	if (bIsStealth)
	{
		OffStealth();
	}
	

	//Animation Setting
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RushAttackMontage, AttackSpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterPlayer::RushAttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RushAttackMontage);
	BurstEffect->ActivateSystem();
	RushWeaponEffect_L->ActivateSystem();
	RushWeaponEffect_R->ActivateSystem();
	RushWindEffect->ActivateSystem();
	MpEffect_LH->ActivateSystem();
	MpEffect_RH->ActivateSystem();
	MpEffect_LF->ActivateSystem();
	MpEffect_RF->ActivateSystem();
}

void ASLCharacterPlayer::RushAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsCanceling)
		return;
	bIsActing = false;
}

void ASLCharacterPlayer::RushAttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(RushAttack), false, this);

	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("RushAttack")));

	const float AttackRange = CharacterSkillData.AttackRange;
	const float AttackRadius = CharacterSkillData.AttackRadius;
	const float AttackDamage = Stat->GetTotalStat().Attack * CharacterSkillData.AttackDamage;
	const FVector Start = GetActorLocation() + (GetActorForwardVector() * (-0.5f * AttackRange));
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_SLACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = (AttackRange * 0.5f) + AttackRadius;
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 2.0f);
#endif
}

void ASLCharacterPlayer::TargetingThrowingKnife()
{
	if (bIsActing) return;

	if (bIsTargeting)return;

	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("ThrowKnife")));
	if (!CheckMP(CharacterSkillData.Mp)) return;

	bIsActing = true;
	bIsTargeting = true;
	
	const float SpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ThrowingKnifeMontage, SpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterPlayer::ThrowingKnifeEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowingKnifeMontage);
	TargetingEffect->Activate(true);
	TargetingEffect->ActivateSystem();
	//MpEffect_LH->ActivateSystem();
	//MpEffect_RH->ActivateSystem();
	//MpEffect_LF->ActivateSystem();
	//MpEffect_RF->ActivateSystem();
}

void ASLCharacterPlayer::CancelThrowingKnife()
{
	if (!bIsTargeting)return;	

	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("ThrowKnife")));
	Stat->HealMp(CharacterSkillData.Mp);

	const float SpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CancelThrowingKnifeMontage, SpeedRate);
}

void ASLCharacterPlayer::ThrowKnife()
{
	if (!bIsTargeting)return;
	bIsTargeting = false;

	if (bIsStealth)
	{
		OffStealth();
	}
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FName NextSection = TEXT("Throw");
	AnimInstance->Montage_JumpToSection(NextSection, ThrowingKnifeMontage);
}

void ASLCharacterPlayer::ThrowingKnifeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsTargeting = false;

	TargetingEffect->Activate(false);
	TargetingEffect->DeactivateSystem();
	//TargetingEffect->SetVisibility(false);

	if (bIsCanceling)
		return;

	bIsActing = false;
}

void ASLCharacterPlayer::ShadowStrike()
{
	if (bIsActing || !bIsStealth) return;
	
	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("ShadowStrike")));
	if (!CheckMP(CharacterSkillData.Mp)) return;

	bIsActing = true;

	//UE_LOG(LogTemp, Log, TEXT("ShadowStrike"));

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	//Animation Setting
	const float AttackSpeedRate = 1.2f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ShadowStrikeMontage, AttackSpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterPlayer::ShadowStrikeEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ShadowStrikeMontage);
	ShadowStrikeWeaponEffect_L->ActivateSystem();
	ShadowStrikeWeaponEffect_R->ActivateSystem();
	ShadowStrikeBodyEffect->ActivateSystem();
	BloodEffect_LH->ActivateSystem();
	BloodEffect_RH->ActivateSystem();
	BloodEffect_LF->ActivateSystem();
	BloodEffect_RF->ActivateSystem();
}

void ASLCharacterPlayer::ShadowStrikeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	if (bIsCanceling)
		return;

	bIsActing = false;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ASLCharacterPlayer::ShadowStrikeHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(ShadowStrike), false, this);

	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("ShadowStrike")));
	const float AttackRange = CharacterSkillData.AttackRange;
	const float AttackRadius = CharacterSkillData.AttackRadius;
	const float AttackDamage = Stat->GetTotalStat().Attack * CharacterSkillData.AttackDamage;
	const FVector Start = GetActorLocation() + (GetActorForwardVector() * AttackRadius);
	const FVector End = Start + GetActorForwardVector() * AttackRange;
	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_SLACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = (AttackRange * 0.5f) + AttackRadius;
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 2.0f);
#endif
}

void ASLCharacterPlayer::Evade()
{
	if (bIsCanceling) return;

	FSLCharacterSkillData CharacterSkillData(USLGameSingleton::Get().GetCharacterSkillData(TEXT("Evade")));
	if (!CheckMP(CharacterSkillData.Mp)) return;

	bIsCanceling = true;
		
	if (bIsStealth)
	{
		OffStealth();
	}

	// Movement Setting
	bIsActing = true;

	//Animation Setting
	const float EvadeSpeedRate = 1.3f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(EvadeMontage, EvadeSpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterPlayer::EvadeEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, EvadeMontage);
	DodgeEffect1->ActivateSystem();
	FVector BoneLocation = GetMesh()->GetBoneLocation(TEXT("pelvis"));
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), DodgeEffect2->Template, BoneLocation, GetActorRotation());
	//MpEffect_LH->ActivateSystem();
	//MpEffect_RH->ActivateSystem();
	//MpEffect_LF->ActivateSystem();
	//MpEffect_RF->ActivateSystem();
}

void ASLCharacterPlayer::EvadeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	bIsActing = false;
	bIsCanceling = false;
}

void ASLCharacterPlayer::ToggleLockOn()
{
	if (bIsLockOn)
	{
		LockOff();
	}
	else
	{
		LockOn();
	}
}

void ASLCharacterPlayer::LockOn()
{
	if (TargetSystem->DetectNearestTarget())
	{
		bIsLockOn = true;
	}
}

void ASLCharacterPlayer::LockOff()
{
	TargetSystem->RemoveTarget();
}

void ASLCharacterPlayer::LockOnTick(float DelataTime)
{
	if (!bIsLockOn)
		return;

	APawn* Target = TargetSystem->GetCurrentTarget();
	if (!Target || !CameraBoom)
		return;
	
	APlayerController* PlayerController = Cast<APlayerController>(GetController());
	if (!PlayerController || !PlayerController->PlayerCameraManager) return;
	
	FVector PlayerLocation = GetActorLocation();
	FVector TargetLocation = Target->GetActorLocation();
	FVector LookAtLocation = (PlayerLocation + TargetLocation) * 0.5f;
	
	FVector CameraPosition = PlayerController->PlayerCameraManager->GetCameraLocation();
	
	FVector DesiredDirection = (LookAtLocation - CameraPosition).GetSafeNormal();
	FRotator DesiredRotation = DesiredDirection.Rotation();

	float Distance = FVector::Dist(PlayerLocation, TargetLocation);
	const float MinDistance = 0.f;
	const float MaxDistance = TargetSystem->GetMaxDistance();
	const float MaxPitch = -5.f;      //수평
	const float MinPitch = -45.f;
	float TargetPitch = FMath::GetMappedRangeValueClamped(FVector2D(MinDistance, MaxDistance), FVector2D(MinPitch, MaxPitch), Distance);

	DesiredRotation.Pitch = TargetPitch;
	FRotator CurrentRotation = PlayerController->GetControlRotation();

	float RotationInterpSpeed = 5.f; // 부드러운 전환 속도
	FRotator NewRotation = FMath::RInterpTo(CurrentRotation, DesiredRotation, GetWorld()->GetDeltaSeconds(), RotationInterpSpeed);
	
	Controller->SetControlRotation(NewRotation);
}

void ASLCharacterPlayer::UpdateTarget(APawn* InPawn)
{
	if (InPawn == nullptr)
	{
		bIsLockOn = false;

	}
	else
	{
		
	}
}
