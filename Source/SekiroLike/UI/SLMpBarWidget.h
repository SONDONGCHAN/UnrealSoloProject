// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UI/SLUserWidget.h"
#include "GameData/SLCharacterStat.h"
#include "SLMpBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

UCLASS()
class SEKIROLIKE_API USLMpBarWidget : public USLUserWidget
{
	GENERATED_BODY()
	
public:
	USLMpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat);
	void UpdateCurrentMp(float NewCurrentMp);
	FString GetMpStatText();

protected:
	void UpdateMpBar();

protected:
	UPROPERTY()
	TObjectPtr<UProgressBar> MpProgressBar;

	UPROPERTY()
	TObjectPtr<UTextBlock> MpStat;

	UPROPERTY()
	float MaxMp;

	UPROPERTY()
	float CurrentMp;
};
