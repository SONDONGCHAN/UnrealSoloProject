// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/SLCharacterStat.h"
#include "SLCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/ );
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FSLCharacterStat& /*BaseStat*/, const FSLCharacterStat& /*ModifierStat*/);



UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEKIROLIKE_API USLCharacterStatComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLCharacterStatComponent();

protected:
	// Called when the game starts
	virtual void InitializeComponent() override;
	virtual void BeginPlay() override;

public:
	FOnHpZeroDelegate OnHpZero;
	FOnHpChangedDelegate OnHpChanged;
	FOnStatChangedDelegate OnStatChanged;

	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	FORCEINLINE void SetBaseStat(const FSLCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE const FSLCharacterStat& GetBaseStat() const { return BaseStat; }
	FORCEINLINE void SetModifierStat(const FSLCharacterStat& InModifierStat) { ModifierStat = InModifierStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE const FSLCharacterStat& GetModifierStat() const { return ModifierStat; }
	FORCEINLINE FSLCharacterStat GetTotalStat() const { return BaseStat + ModifierStat; }

	FORCEINLINE void SetStatMultipleValue(float InMultipleValue) { CurrentStatMultipleValue *= InMultipleValue; }
	FORCEINLINE float GetStatMultipleValue() { return CurrentStatMultipleValue; }

	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);
	
	// 구현 필요
	FORCEINLINE float GetCurrentMp() { return CurrentMp; }
	float ApplyMpConsumption(float InMpConsumption);
	
	// 구현 필요
	FORCEINLINE float GetCurrentExp() { return CurrentExp; }
	float ApplyExpAdd(float InExpAdd);

protected:
	void SetHP(float NewHp);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	// 구현 필요
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;

	// 구현 필요
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentExp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	// 구현 필요
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentStatMultipleValue;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FSLCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FSLCharacterStat ModifierStat;
};
