// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SLCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "SLComboActionData.h"
#include "Physics/SLCollision.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/SLCharacterStatComponent.h"
//#include "UI/SLWidgetComponent.h"
#include "Item/SLItems.h"
#include "GameData/SLGameSingleton.h"
#include "GameData/SLCharacterComboData.h"
#include "GameData/SLGeneralData.h"
#include "Projectile/SLProjectile.h"
#include "Projectile/SLProjectilePoolManager.h"
#include "Curve/SLCurveManager.h"

DEFINE_LOG_CATEGORY(LogSLCharacter);

// Sets default values
ASLCharacterBase::ASLCharacterBase()
{
	// Pawn
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	//Capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
	GetCapsuleComponent()->SetCollisionProfileName(CPROFILE_SLCAPSULE);

	//Movement
	GetCharacterMovement()->bOrientRotationToMovement = true; 
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 400.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 800.f;
	GetCharacterMovement()->MaxAcceleration = 1000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USLCharacterControlData> ShoulderDataRef(TEXT("/Script/SekiroLike.SLCharacterControlData'/Game/SekiroLike/CharacterControl/SLC_Shoulder.SLC_Shoulder'"));
	if (ShoulderDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Shoulder, ShoulderDataRef.Object);
	}
	
	static ConstructorHelpers::FObjectFinder<USLCharacterControlData> QuaterDataRef(TEXT("/Script/SekiroLike.SLCharacterControlData'/Game/SekiroLike/CharacterControl/SLC_Quarter.SLC_Quarter'"));
	if (QuaterDataRef.Object)
	{
		CharacterControlManager.Add(ECharacterControlType::Quater, QuaterDataRef.Object);
	}

	static ConstructorHelpers::FObjectFinder<USLComboActionData> ComboActionDataRef(TEXT("/Script/SekiroLike.SLComboActionData'/Game/SekiroLike/CharacterAction/SLA_ComboAttack.SLA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<USLCharacterStatComponent>(TEXT("Stat"));

	//Item Actions
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ASLCharacterBase::DrinkPotion)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ASLCharacterBase::BoostPower)));
	TakeItemActions.Add(FTakeItemDelegateWrapper(FOnTakeItemDelegate::CreateUObject(this, &ASLCharacterBase::EnhanceStat)));


}

void ASLCharacterBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	Stat->OnHpZero.AddUObject(this, &ASLCharacterBase::SetDeath);
	Stat->OnStatChanged.AddUObject(this, &ASLCharacterBase::ApplyStat);
}

void ASLCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	DodgeTick(DeltaTime);
}

void ASLCharacterBase::SetCharacterControlData(const USLCharacterControlData* CharacterControlData)
{
	// Pawn
	bUseControllerRotationYaw = CharacterControlData->bUseControllerRotationYaw;

	// CharacterMovement
	GetCharacterMovement()->bOrientRotationToMovement = CharacterControlData->bOrientRotationToMovement;
	GetCharacterMovement()->bUseControllerDesiredRotation = CharacterControlData->bUseControllerDesiredRotation;
	GetCharacterMovement()->RotationRate = CharacterControlData->RotationRate;
}

void ASLCharacterBase::ProcessComboCommand()
{
	if (CurrentCombo == 0)
	{
		ComboActionBegin();
		return;
	}

	if (!ComboTimerHandle.IsValid())
	{
		HasNextComboCommand = false;
	}
	else
	{
		HasNextComboCommand = true;
	}
}

void ASLCharacterBase::ComboActionBegin()
{
	//Combo Status
	CurrentCombo = 1;

	// Movement Setting
	//GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	
	//Animation Setting
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, ComboActionData->AnimFPS[0]);
	FOnMontageEnded EndDelegate;
	EndDelegate.BindUObject(this, &ASLCharacterBase::ComboActionEnd);
	AnimInstance->Montage_SetEndDelegate(EndDelegate, ComboActionMontage);

	ComboTimerHandle.Invalidate();
	SetComboCheckTimer();
}

void ASLCharacterBase::ComboActionEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded)
{
	ensure(CurrentCombo != 0);
	CurrentCombo = 0;
	HasNextComboCommand = false;

	NotifyComboActionEnd();
}

void ASLCharacterBase::NotifyComboActionEnd()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}

void ASLCharacterBase::SetComboCheckTimer()
{
	int32 ComboIndex = CurrentCombo - 1;
	ensure(ComboActionData->EffectiveFrameCount.IsValidIndex(ComboIndex));
	
	const float AttackSpeedRate = 1.f;

	float SectionStart = ComboActionMontage->CompositeSections[ComboIndex].GetTime();
	int NextIndex = FMath::Clamp(ComboIndex + 1, 0, ComboActionData->MaxComboCount - 1);
	float SectionNext = ComboActionMontage->CompositeSections[NextIndex].GetTime();
	float Length = SectionNext - SectionStart;

	//UE_LOG(LogTemp, Warning, TEXT("Section Length : %f"), Length);

	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	//UE_LOG(LogTemp, Warning, TEXT("Timer Length : %f"), ComboEffectiveTime);

	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ASLCharacterBase::ComboCheck, ComboEffectiveTime, false);
		//UE_LOG(LogTemp, Warning, TEXT("Set Combo %d Timerd"), CurrentCombo);
	}
}

void ASLCharacterBase::ComboCheck()
{
	ComboTimerHandle.Invalidate();
	if (HasNextComboCommand)
	{
		UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
		CurrentCombo = FMath::Clamp(CurrentCombo + 1, 1 , ComboActionData->MaxComboCount);
		FName NextSection = *FString::Printf(TEXT("%s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		AnimInstance->Montage_JumpToSection(NextSection, ComboActionMontage);
		AnimInstance->Montage_SetPlayRate(ComboActionMontage, ComboActionData->AnimFPS[CurrentCombo-1]);
		SetComboCheckTimer();
		HasNextComboCommand = false;
		return;
	}
}

FHitResult ASLCharacterBase::ComboAttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FSLCharacterComboData CharacterComboData(USLGameSingleton::Get().GetCharacterComboData(CharacterType, CurrentCombo));
	
	const float AttackRange = CharacterComboData.AttackRange;
	const float AttackRadius = CharacterComboData.AttackRadius;
	const float AttackDamage = Stat->GetTotalStat().Attack * CharacterComboData.AttackDamage;
	const FVector Start = GetActorLocation() + ( GetActorForwardVector() * (GetCapsuleComponent()->GetScaledCapsuleRadius() + AttackRadius) );
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
	return OutHitResult;
}

void ASLCharacterBase::ShootProjectile(float ShootPower, EProjectileType MyProjectileType)
{
	FVector SpawnLocation = GetMesh()->GetSocketLocation("dagger_a_r");
	FRotator SpawnRotation = GetActorRotation(); // 카메라/컨트롤러 방향 등

	ASLProjectile* Projectile = ASLProjectilePoolManager::GetInstance(GetWorld())->GetProjectile(MyProjectileType);
	if (Projectile)
	{
		Projectile->ActiveProjectile();
		Projectile->SetActorLocationAndRotation(SpawnLocation, SpawnRotation);
		FVector Direction = SpawnRotation.Vector();
		Projectile->FireInDirection(Direction, ShootPower);
		Projectile->SetOwner(this);
	}
}

float ASLCharacterBase::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser); // EventInstigator == 가해자, DamageCauser == 가해자가 방의한 폰 또는 무기
	
	Stat->ApplyDamage(DamageAmount);
	
	return DamageAmount;
}

void ASLCharacterBase::SetDeath()
{
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);
	PlayDeathAnimation();
	SetActorEnableCollision(false);
	
}

void ASLCharacterBase::PlayDeathAnimation()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance)
	{
		AnimInstance->StopAllMontages(0.f);
		AnimInstance->Montage_Play(DeathMontage, 1.f);
	}
}



void ASLCharacterBase::TakeItem(USLItemData* InItemData)
{
	if (InItemData)
	{
		TakeItemActions[(uint8)InItemData->Type].ItemDelegate.ExecuteIfBound(InItemData);
	}
}

void ASLCharacterBase::DrinkPotion(USLItemData* InItemData)
{
	USLPotionItemData* PotionItemData = Cast<USLPotionItemData>(InItemData);
	if (PotionItemData)
	{
		switch (PotionItemData->PotionType)
		{
		case EPotionType::Hp :
			Stat->HealHp(PotionItemData->HealAmount);
			UE_LOG(LogSLCharacter, Log, TEXT("Drink Hp Potion : %f"), PotionItemData->HealAmount);
			return;

		case EPotionType::Mp:
			Stat->HealMp(PotionItemData->HealAmount);
			UE_LOG(LogSLCharacter, Log, TEXT("Drink Mp Potion : %f"), PotionItemData->HealAmount);
			return;
		}		
	}
}

void ASLCharacterBase::BoostPower(USLItemData* InItemData)
{
	USLPowerBoosterItemData* PowerBoosterItemData = Cast<USLPowerBoosterItemData>(InItemData);
	if (PowerBoosterItemData)
	{
		Stat->AddBoostAttackMag(PowerBoosterItemData->BoostAmount, PowerBoosterItemData->BoostTime);
	}

	UE_LOG(LogSLCharacter, Log, TEXT("Boost Power"));
}

void ASLCharacterBase::EnhanceStat(USLItemData* InItemData)
{
	USLStatEnhancerItemData* StatEnhancerItemData = Cast<USLStatEnhancerItemData>(InItemData);
	if (StatEnhancerItemData)
	{
		Stat->AddModifierStat(StatEnhancerItemData->ModifierStat);
	}

	UE_LOG(LogSLCharacter, Log, TEXT("Enhance Stat"));
}

int32 ASLCharacterBase::GetLevel()
{
	return int32();
}

void ASLCharacterBase::SetLevel(int32 InNewLevel)
{
}

void ASLCharacterBase::ApplyStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
}

void ASLCharacterBase::Dodge(const FVector DodgeDir, const float StartSpeed, const float DodgeTime, const ECurveType CurveType)
{
	bIsDodge = true;
	DodgeDirection = DodgeDir * StartSpeed;
	DodgeTimeTotal = DodgeTime;
	DodgeTimeLeft = DodgeTimeTotal;
	CurrentCurveType = CurveType;
	GetCharacterMovement()->StopMovementImmediately();
}

void ASLCharacterBase::DodgeTick(float DeltaTime)
{
	if (!bIsDodge)
		return;

	UCurveFloat* Curve = Cast<UCurveFloat>(ASLCurveManager::GetInstance(GetWorld())->GetCurve(CurrentCurveType));
	if (Curve)
	{
		float Ratio = 1.f - (DodgeTimeLeft / DodgeTimeTotal);
		float CurveResult = Curve->GetFloatValue(Ratio);
		AddActorWorldOffset(DodgeDirection * CurveResult * DeltaTime, true);
		//UE_LOG(LogTemp, Log, TEXT("CurveResult : %f"), CurveResult);
	}

	DodgeTimeLeft -= DeltaTime;
	if (DodgeTimeLeft <= 0.f)
	{
		bIsDodge = false;
	}
}

bool ASLCharacterBase::CanDetect()
{
	return true;
}
