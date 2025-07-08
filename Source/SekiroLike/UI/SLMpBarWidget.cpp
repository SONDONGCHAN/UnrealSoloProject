// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLMpBarWidget.h"
#include "Components/ProgressBar.h"
#include "Components/TextBlock.h"
#include "Interface/SLCharacterWidgetInterface.h"

USLMpBarWidget::USLMpBarWidget(const FObjectInitializer& ObjectInitializer)
{
	MaxMp = -1;
}

void USLMpBarWidget::NativeConstruct()
{
	Super::NativeConstruct();

	MpProgressBar = Cast<UProgressBar>(GetWidgetFromName(TEXT("PBMpBar")));
	ensure(MpProgressBar);

	MpStat = Cast<UTextBlock>(GetWidgetFromName(TEXT("TxtMpStat")));
	ensure(MpStat);
}

void USLMpBarWidget::UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
	MaxMp = (BaseStat + ModifierStat).MaxMp;
	UpdateMpBar();
}

void USLMpBarWidget::UpdateCurrentMp(float NewCurrentMp)
{
	CurrentMp = NewCurrentMp;
	UpdateMpBar();
}

FString USLMpBarWidget::GetMpStatText()
{
	return FString::Printf(TEXT("%.0f/%.0f"), CurrentMp, MaxMp);
}

void USLMpBarWidget::UpdateMpBar()
{

	if (MpProgressBar)
	{
		MpProgressBar->SetPercent(CurrentMp / MaxMp);
	}

	if (MpStat)
	{
		MpStat->SetText(FText::FromString(GetMpStatText()));
	}	
}