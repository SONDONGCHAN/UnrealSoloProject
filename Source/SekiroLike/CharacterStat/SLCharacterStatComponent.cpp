// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/SLCharacterStatComponent.h"
#include "GameData/SLGameSingleton.h"


// Sets default values for this component's properties
USLCharacterStatComponent::USLCharacterStatComponent()
{
	CurrentLevel = 1;
	
	bWantsInitializeComponent = true; // For Call InitializeComponent 
}

void USLCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	SetMp(BaseStat.MaxMp);
	SetBoostAttackMag(0);
}

// Called when the game starts
void USLCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
}

void USLCharacterStatComponent::SetLevelStat(int32 InNewLevel)
{
	CurrentLevel = FMath::Clamp(InNewLevel, 1, USLGameSingleton::Get().CharacterMaxLevel);
	SetBaseStat(USLGameSingleton::Get().GetCharacterStat(CurrentLevel));
	check(BaseStat.MaxHp > 0.f);
	SetBoostAttack();
}

void USLCharacterStatComponent::AddModifierStat(const FSLCharacterStat& InModifierStat)
{
	FSLCharacterStat ActualModifierStat = ModifierStat + InModifierStat;
	SetModifierStat(ActualModifierStat);
}

void USLCharacterStatComponent::AddBoostAttackMag(float InAddAmount, float InBoostTime)
{
	const float ActualBoostAttack = FMath::Max(0.f, BoostAttackMag + InAddAmount);
	const float ActualBoostAttackAdd = ActualBoostAttack - BoostAttackMag;
	SetBoostAttackMag(ActualBoostAttack);

	FTimerHandle BoostEndTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(BoostEndTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			ReduceBoostAttackMag(ActualBoostAttackAdd);
		}
	), InBoostTime, false);
}

void USLCharacterStatComponent::ReduceBoostAttackMag(float InReduceAmount)
{
	const float ActualBoostAttack = FMath::Max(0, BoostAttackMag - InReduceAmount);
	SetBoostAttackMag(ActualBoostAttack);
}

float USLCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float ActualDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);

	SetHp(PrevHp - ActualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return ActualDamage;
}

void USLCharacterStatComponent::HealHp(float InHealAmount)
{
	const float ActualHp = FMath::Clamp(CurrentHp + InHealAmount, CurrentHp, GetTotalStat().MaxHp);
	SetHp(ActualHp);
}

float USLCharacterStatComponent::ApplyMpConsumption(float InMpConsumption)
{
	const float PrevMp = CurrentMp;
	const float AxtualMpConsumption = FMath::Clamp<float>(InMpConsumption, 0.f, InMpConsumption);

	SetMp(PrevMp - AxtualMpConsumption);

	return AxtualMpConsumption;
}

void USLCharacterStatComponent::HealMp(float InHealAmount)
{
	const float ActualMp = FMath::Clamp(CurrentMp + InHealAmount, CurrentMp, GetTotalStat().MaxMp);
	SetMp(ActualMp);
}

float USLCharacterStatComponent::ApplyExpAdd(float InExpAdd)
{
	return 0.0f;
}

void USLCharacterStatComponent::SetModifierStat(const FSLCharacterStat& InModifierStat)
{
	ModifierStat = InModifierStat;
	OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat());
	SetBoostAttack();
}

void USLCharacterStatComponent::SetBoostAttackMag(float InBoostAttackMag)
{
	BoostAttackMag = InBoostAttackMag;
	SetBoostAttack();
}

void USLCharacterStatComponent::SetBoostAttack()
{
	float AxtualBoostAttack = ((BaseStat + ModifierStat).Attack ) * (GetBoostAttackMag());
	BoostStat.Attack = AxtualBoostAttack;
	OnBoostStatChanged.Broadcast(GetBoostStat());
}

void USLCharacterStatComponent::SetHp(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, BaseStat.MaxHp);
	OnHpChanged.Broadcast(CurrentHp);
}

void USLCharacterStatComponent::SetMp(float NewMp)
{
	CurrentMp = FMath::Clamp<float>(NewMp, 0.f, BaseStat.MaxMp);
	OnMpChanged.Broadcast(CurrentMp);
}

void USLCharacterStatComponent::SetExp(float NewExp)
{

}

void USLCharacterStatComponent::SetLevel(float NewLevel)
{
}
