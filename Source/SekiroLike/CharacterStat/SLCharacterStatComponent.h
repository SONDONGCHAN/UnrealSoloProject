// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "GameData/SLCharacterStat.h"
#include "SLCharacterStatComponent.generated.h"

DECLARE_MULTICAST_DELEGATE(FOnHpZeroDelegate);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnHpChangedDelegate, float /*CurrentHp*/ );
DECLARE_MULTICAST_DELEGATE_OneParam(FOnMpChangedDelegate, float /*CurrentMp*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnExpChangedDelegate, float /*CurrentExp*/);
DECLARE_MULTICAST_DELEGATE_TwoParams(FOnStatChangedDelegate, const FSLCharacterStat& /*BaseStat*/, const FSLCharacterStat& /*ModifierStat*/);
DECLARE_MULTICAST_DELEGATE_OneParam(FOnBoostStatChangedDelegate, const FSLCharacterStat& /*BoostStat*/);

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
	FOnMpChangedDelegate OnMpChanged;
	FOnExpChangedDelegate OnExpChanged;
	FOnStatChangedDelegate OnStatChanged;
	FOnBoostStatChangedDelegate OnBoostStatChanged;


	void SetLevelStat(int32 InNewLevel);
	FORCEINLINE float GetCurrentLevel() const { return CurrentLevel; }

	FORCEINLINE void SetBaseStat(const FSLCharacterStat& InBaseStat) { BaseStat = InBaseStat; OnStatChanged.Broadcast(GetBaseStat(), GetModifierStat()); }
	FORCEINLINE const FSLCharacterStat& GetBaseStat() const { return BaseStat; }

	FORCEINLINE const FSLCharacterStat& GetModifierStat() const { return ModifierStat; }
	void AddModifierStat(const FSLCharacterStat& InModifierStat);

	FORCEINLINE FSLCharacterStat GetTotalStat() const { return BaseStat + ModifierStat + BoostStat; }

	FORCEINLINE float GetBoostAttackMag() { return BoostAttackMag; }
	void AddBoostAttackMag(float InAddAmount, float InBoostTime);
	void ReduceBoostAttackMag(float InReduceAmount);

	FORCEINLINE const FSLCharacterStat& GetBoostStat() const { return BoostStat; }

	FORCEINLINE float GetCurrentHp() { return CurrentHp; }
	float ApplyDamage(float InDamage);
	void HealHp(float InHealAmount);
	
	FORCEINLINE float GetCurrentMp() { return CurrentMp; }
	float ApplyMpConsumption(float InMpConsumption);
	void HealMp(float InHealAmount);

	// 구현 필요
	FORCEINLINE float GetCurrentExp() { return CurrentExp; }
	float ApplyExpAdd(float InExpAdd);

protected:
	void SetModifierStat(const FSLCharacterStat& InModifierStat);
	void SetBoostAttackMag(float InBoostAttackMag);
	void SetBoostAttack();
	void SetHp(float NewHp);
	void SetMp(float NewMp);

	// 구현 필요
	void SetExp(float NewExp);
	// 구현 필요
	void SetLevel(float NewLevel);

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentHp;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentMp;

	// 구현 필요
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentExp;

	// 구현 필요
	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float CurrentLevel;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FSLCharacterStat BaseStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FSLCharacterStat ModifierStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat, Meta = (AllowPrivateAccess = "true"))
	FSLCharacterStat BoostStat;

	UPROPERTY(Transient, VisibleInstanceOnly, Category = Stat)
	float BoostAttackMag;
};