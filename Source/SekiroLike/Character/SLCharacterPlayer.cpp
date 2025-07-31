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

ASLCharacterPlayer::ASLCharacterPlayer()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller
	
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false;
	//여기에 이름을 붙일 때 소켓으로 붙임. Spring Arm 끝에 붙는 것임.

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

	CurrentCharacterControlType = ECharacterControlType::Shoulder;


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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> CancleThrowingKnifeMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_CancleThrowingKnife.AM_CancleThrowingKnife'"));
	if (CancleThrowingKnifeMontageRef.Object)
	{
		CancleThrowingKnifeMontage = CancleThrowingKnifeMontageRef.Object;
	}	
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
	
	EnhancedInputComponent->BindAction(StealthWalkAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::ToggleStealthMode);
	
	//Skill
	EnhancedInputComponent->BindAction(RushAttackAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::RushAttack);
	
	//Skill
	EnhancedInputComponent->BindAction(TargetingThrowingKnifeAction, ETriggerEvent::Started, this, &ASLCharacterPlayer::TargetingThrowingKnife);
	EnhancedInputComponent->BindAction(TargetingThrowingKnifeAction, ETriggerEvent::Completed, this, &ASLCharacterPlayer::CancleThrowingKnife);
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
	if (isTargeting)
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
	if (bIsStealth)
		return;
	UE_LOG(LogTemp, Log, TEXT("StartSprint"));
	GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
	TargetSpeed = SprintSpeed;
}

void ASLCharacterPlayer::EndSprint()
{
	if (bIsStealth)
		return;
	UE_LOG(LogTemp, Log, TEXT("EndSprint"));
	bIsSmoothing = true;
	TargetSpeed = NormalSpeed;
}

void ASLCharacterPlayer::ToggleStealthMode()
{
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
	bIsStealth = true;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
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

}

void ASLCharacterPlayer::RushAttack()
{
	UE_LOG(LogTemp, Log, TEXT("RushAttack"));

	// Movement Setting
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);

	//Animation Setting
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(RushAttackMontage, AttackSpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterPlayer::RushAttackEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, RushAttackMontage);
}

void ASLCharacterPlayer::RushAttackEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
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
	if(isTargeting)
		return;

	isTargeting = true;
	UE_LOG(LogTemp, Log, TEXT("TargetingStart"));
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	const float SpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ThrowingKnifeMontage, SpeedRate);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterPlayer::ThrowingKnifeEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ThrowingKnifeMontage);
}

void ASLCharacterPlayer::CancleThrowingKnife()
{
	if (!isTargeting)
		return;
	
	isTargeting = false;
	UE_LOG(LogTemp, Log, TEXT("TargetingCancle"));

	const float SpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(CancleThrowingKnifeMontage, SpeedRate);
}

void ASLCharacterPlayer::ThrowKnife()
{
	if (!isTargeting)
		return;
	
	isTargeting = false;
	UE_LOG(LogTemp, Log, TEXT("ThrowKnife"));
	
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	FName NextSection = TEXT("Throw");
	AnimInstance->Montage_JumpToSection(NextSection, ThrowingKnifeMontage);
}

void ASLCharacterPlayer::ThrowingKnifeEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	UE_LOG(LogTemp, Log, TEXT("TargetingEnd"));
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ASLCharacterPlayer::SkillTick(float DeltaTime)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		if (Cast<USLAnimInstance>(AnimInstance)->GetIsSprinting())
		{
			Stat->ApplyMpConsumption(3.f * DeltaTime);
		}
	}

	if (bIsSmoothing)
	{
		float CurrentSpeed = GetCharacterMovement()->MaxWalkSpeed;
	
		float Epsilon = 2.f;
		if (FMath::Abs(TargetSpeed - CurrentSpeed) < Epsilon)
		{
			bIsSmoothing = false;
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
}
