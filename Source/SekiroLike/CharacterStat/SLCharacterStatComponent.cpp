// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStat/SLCharacterStatComponent.h"
#include "GameData/SLGameSingleton.h"


// Sets default values for this component's properties
USLCharacterStatComponent::USLCharacterStatComponent()
{
	CurrentLevel = 1;
	CurrentStatMultipleValue = 1.f;

	bWantsInitializeComponent = true; // For Call InitializeComponent 
}


void USLCharacterStatComponent::InitializeComponent()
{
	Super::InitializeComponent();

	SetLevelStat(CurrentLevel);
	SetHp(BaseStat.MaxHp);
	SetMp(BaseStat.MaxMp);

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
}

void USLCharacterStatComponent::AddModifierStat(const FSLCharacterStat& InModifierStat)
{
	FSLCharacterStat ActualModifierStat = ModifierStat + InModifierStat;
	SetModifierStat(ActualModifierStat);
}

void USLCharacterStatComponent::AddStatMultipleValue(float InAddAmount)
{
	const float ActualMultipleValue = FMath::Max(CurrentStatMultipleValue + InAddAmount, CurrentStatMultipleValue);
	SetStatMultipleValue(ActualMultipleValue);
}

void USLCharacterStatComponent::ReduceStatMultipleValue(float InReduceAmount)
{
	const float ActualMultipleValue = FMath::Max(CurrentStatMultipleValue - InReduceAmount, 0);
	SetStatMultipleValue(ActualMultipleValue);
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
}

void USLCharacterStatComponent::SetStatMultipleValue(float InMultipleValue)
{
	CurrentStatMultipleValue = InMultipleValue;
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
