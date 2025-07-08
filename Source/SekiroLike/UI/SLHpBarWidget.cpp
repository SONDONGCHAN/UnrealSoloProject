// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLHpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/SLCharacterWidgetInterface.h"

USLHpBarWidget::USLHpBarWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	MaxHp = -1;
}

void USLHpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBHpBar")));
	ensure(HpProgressBar);

	HpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtHpStat")));
	ensure(HpStat);

	ISLCharacterWidgetInterface* CharactereWidget = Cast<ISLCharacterWidgetInterface>(OwningActor);
	if (CharactereWidget)
	{
		CharactereWidget->SetupCharacterWidget(this);
	}
}

void USLHpBarWidget::UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
	MaxHp = (BaseStat + ModifierStat).MaxHp;
	UpdateHpBar();
}

void USLHpBarWidget::UpdateCurrentHp(float NewCurrentHp)
{
	CurrentHp = NewCurrentHp;
	UpdateHpBar();
}

FString USLHpBarWidget::GetHpStatText()
{
	return FString::Printf(TEXT("%.0f/%.0f"), CurrentHp, MaxHp);
}

void USLHpBarWidget::UpdateHpBar()
{
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(CurrentHp / MaxHp);
	}

	if (HpStat)
	{
		HpStat->SetText(FText::FromString(GetHpStatText()));
	}
}
