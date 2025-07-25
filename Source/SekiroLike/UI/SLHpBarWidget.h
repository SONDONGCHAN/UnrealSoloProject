// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLUserWidget.h"
#include "GameData/SLCharacterStat.h"
#include "SLHpBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SEKIROLIKE_API USLHpBarWidget : public USLUserWidget
{
	GENERATED_BODY()
	
public:
	USLHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat);
	void UpdateCurrentHp(float NewCurrentHp);
	FString GetHpStatText();

protected:
	void UpdateHpBar();

protected:
	UPROPERTY()
	TObjectPtr<UProgressBar> HpProgressBar;

	UPROPERTY()
	TObjectPtr<UTextBlock> HpStat;

	UPROPERTY()
	float MaxHp;

	UPROPERTY()
	float CurrentHp;
};
