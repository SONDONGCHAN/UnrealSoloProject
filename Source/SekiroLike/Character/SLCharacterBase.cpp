// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterBase.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "SLCharacterControlData.h"
#include "Animation/AnimMontage.h"
#include "SLComboActionData.h"
#include "Physics/SLCollision.h"
#include "Engine/DamageEvents.h"
#include "Engine/DamageEvents.h"
#include "CharacterStat/SLCharacterStatComponent.h"
#include "UI/SLWidgetComponent.h"
#include "UI/SLHpBarWidget.h"
#include "Item/SLItems.h"


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
	GetCharacterMovement()->MaxWalkSpeed = 450;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Mesh
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -100.0f), FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	GetMesh()->SetCollisionProfileName(TEXT("NoCollision"));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> CharacterSkeletalMesh(TEXT("/Script/Engine.SkeletalMesh'/Game/ParagonKallari/Characters/Heroes/Kallari/Meshes/Kallari.Kallari'"));
	if (CharacterSkeletalMesh.Object)
	{
		GetMesh()->SetSkeletalMesh(CharacterSkeletalMesh.Object);
	}
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> AnimInstanceRef(TEXT("/Game/SekiroLike/Animation/ABP_SLCharacter.ABP_SLCharacter_C"));
	if (AnimInstanceRef.Class)
	{
		GetMesh()->SetAnimInstanceClass(AnimInstanceRef.Class);
	}

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

	static ConstructorHelpers::FObjectFinder<UAnimMontage> ComboActionMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_ComboAttack.AM_ComboAttack'"));
	if (ComboActionMontageRef.Object)
	{
		ComboActionMontage = ComboActionMontageRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<USLComboActionData> ComboActionDataRef(TEXT("/Script/SekiroLike.SLComboActionData'/Game/SekiroLike/CharacterAction/SLA_ComboAttack.SLA_ComboAttack'"));
	if (ComboActionDataRef.Object)
	{
		ComboActionData = ComboActionDataRef.Object;
	}

	static ConstructorHelpers::FObjectFinder<UAnimMontage> DeadMontageRef(TEXT("/Script/Engine.AnimMontage'/Game/SekiroLike/Animation/AM_Death.AM_Death'"));
	if (DeadMontageRef.Object)
	{
		DeathMontage = DeadMontageRef.Object;
	}

	// Stat Component
	Stat = CreateDefaultSubobject<USLCharacterStatComponent>(TEXT("Stat"));

	// Widget Component
	HpBar = CreateDefaultSubobject<USLWidgetComponent>(TEXT("Widget"));
	HpBar->SetupAttachment(GetMesh());
	HpBar->SetRelativeLocation(FVector(0.0f, 0.0f, 210.0f));
	static ConstructorHelpers::FClassFinder<UUserWidget> HpBarWidgetRef(TEXT("/Game/SekiroLike/UI/WBP_HpBar.WBP_HpBar_C"));
	if (HpBarWidgetRef.Class)
	{
		HpBar->SetWidgetClass(HpBarWidgetRef.Class);
		HpBar->SetWidgetSpace(EWidgetSpace::Screen);
		HpBar->SetDrawSize(FVector2D(150.0f, 15.0f));
		HpBar->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_None);

	//Animation Setting
	const float AttackSpeedRate = 1.f;
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	AnimInstance->Montage_Play(ComboActionMontage, AttackSpeedRate);
	
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
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);

	NotifyComboActionEnd();
}

void ASLCharacterBase::NotifyComboActionEnd()
{

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

	UE_LOG(LogTemp, Warning, TEXT("Section Length : %f"), Length);

	float ComboEffectiveTime = (ComboActionData->EffectiveFrameCount[ComboIndex] / ComboActionData->FrameRate) / AttackSpeedRate;
	UE_LOG(LogTemp, Warning, TEXT("Timer Length : %f"), ComboEffectiveTime);

	if (ComboEffectiveTime > 0.f)
	{
		GetWorld()->GetTimerManager().SetTimer(ComboTimerHandle, this, &ASLCharacterBase::ComboCheck, ComboEffectiveTime, false);
		UE_LOG(LogTemp, Warning, TEXT("Set Combo %d Timerd"), CurrentCombo);
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
		SetComboCheckTimer();
		HasNextComboCommand = false;
		UE_LOG(LogTemp, Warning, TEXT("Input %s%d"), *ComboActionData->MontageSectionNamePrefix, CurrentCombo);
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Nothing Input During Combo %d"), CurrentCombo);
}

void ASLCharacterBase::AttackHitCheck()
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	const float AttackRange = 50.0f; // Attack Skill Info에서 받아줄 예정
	const float AttackRadius = 70.0f; // Attack Skill Info에서 받아줄 예정
	const float AttackDamage = Stat->GetTotalStat().Attack * Stat->GetStatMultipleValue();
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
	HpBar->SetHiddenInGame(true);
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

void ASLCharacterBase::SetupCharacterWidget(USLUserWidget* InUserWidget)
{
	USLHpBarWidget* HpBarWidget = Cast<USLHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		HpBarWidget->UpdateCurrentHp(Stat->GetCurrentHp());
		Stat->OnStatChanged.AddUObject(HpBarWidget, &USLHpBarWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(HpBarWidget, &USLHpBarWidget::UpdateCurrentHp);
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
		Stat->AddStatMultipleValue(PowerBoosterItemData->BoostAmount);
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

