// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SLUserWidget.h"
#include "SLHpBarWidget.generated.h"

class UProgressBar;

UCLASS()
class SEKIROLIKE_API USLHpBarWidget : public USLUserWidget
{
	GENERATED_BODY()
	
public:
	USLHpBarWidget(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void NativeConstruct() override;

public:
	FORCEINLINE void SetMaxHp(float NewMaxHp) { MaxHp = NewMaxHp; }
	void UpdateHpBar(float NewCurrentHp);

protected:
	UPROPERTY()
	TObjectPtr<UProgressBar> HpProgressBar;

	UPROPERTY()
	float MaxHp;
};
