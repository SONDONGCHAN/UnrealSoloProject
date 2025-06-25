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
	SetHP(BaseStat.MaxHp);
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

float USLCharacterStatComponent::ApplyDamage(float InDamage)
{
	const float PrevHp = CurrentHp;
	const float AxtualDamage = FMath::Clamp<float>(InDamage, 0.f, InDamage);

	SetHP(PrevHp - AxtualDamage);
	if (CurrentHp <= KINDA_SMALL_NUMBER)
	{
		OnHpZero.Broadcast();
	}

	return AxtualDamage;
}

float USLCharacterStatComponent::ApplyMpConsumption(float InMpConsumption)
{
	return 0.0f;
}

float USLCharacterStatComponent::ApplyExpAdd(float InExpAdd)
{
	return 0.0f;
}

void USLCharacterStatComponent::SetHP(float NewHp)
{
	CurrentHp = FMath::Clamp<float>(NewHp, 0.f, BaseStat.MaxHp);

	OnHpChanged.Broadcast(CurrentHp);
}
