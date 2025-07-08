// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/SLCharacterStat.h"
#include "SLHUDWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLHUDWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	USLHUDWidget(const FObjectInitializer& ObjectInitializer);

public:
	void UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat);
	void UpdateHpBar(float NewCurrentHp);
	void UpdateMpBar(float NewCurrentMp);

protected:
	virtual void NativeConstruct() override;
	
protected:
	UPROPERTY()
	TObjectPtr<class USLHpBarWidget> HpBar;

	UPROPERTY()
	TObjectPtr<class USLMpBarWidget> MpBar;

	UPROPERTY()
	TObjectPtr<class USLCharacterStatWidget> CharacterStat;
};
