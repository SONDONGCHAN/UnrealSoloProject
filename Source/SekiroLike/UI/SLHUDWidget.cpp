// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLHUDWidget.h"
#include "UI/SLHpBarWidget.h"
#include "UI/SLMpBarWidget.h"
#include "UI/SLCharacterStatWidget.h"
#include "Interface/SLCharacterHUDInterface.h"

USLHUDWidget::USLHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLHUDWidget::UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
	FSLCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->UpdateStat(BaseStat, ModifierStat);
	MpBar->UpdateStat(BaseStat, ModifierStat);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void USLHUDWidget::UpdateBoostStat(const FSLCharacterStat& BoostStat)
{
	CharacterStat->UpdateBoostStat(BoostStat);
}

void USLHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	if (HpBar)
	{
		HpBar->UpdateCurrentHp(NewCurrentHp);
	}
}

void USLHUDWidget::UpdateMpBar(float NewCurrentMp)
{
	if (MpBar)
	{
		MpBar->UpdateCurrentMp(NewCurrentMp);
	}
}

void USLHUDWidget::UpdateExpBar(float NewCurrentExp)
{

}

void USLHUDWidget::UpdateBoostAttack(float NewBoostAttack)
{

}

void USLHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<USLHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	MpBar = Cast<USLMpBarWidget>(GetWidgetFromName(TEXT("WidgetMpBar")));
	ensure(MpBar);

	CharacterStat = Cast<USLCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	ISLCharacterHUDInterface* HUDPawn = Cast<ISLCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

}
