// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLHUDWidget.h"
#include "UI/SLHpBarWidget.h"
#include "UI/SLCharacterStatWidget.h"
#include "Interface/SLCharacterHUDInterface.h"

USLHUDWidget::USLHUDWidget(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
}

void USLHUDWidget::UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
	FSLCharacterStat TotalStat = BaseStat + ModifierStat;
	HpBar->SetMaxHp(TotalStat.MaxHp);

	CharacterStat->UpdateStat(BaseStat, ModifierStat);
}

void USLHUDWidget::UpdateHpBar(float NewCurrentHp)
{
	if (HpBar)
	{
		HpBar->UpdateHpBar(NewCurrentHp);
	}
}

void USLHUDWidget::NativeConstruct()
{
	Super::NativeConstruct();

	HpBar = Cast<USLHpBarWidget>(GetWidgetFromName(TEXT("WidgetHpBar")));
	ensure(HpBar);

	CharacterStat = Cast<USLCharacterStatWidget>(GetWidgetFromName(TEXT("WidgetCharacterStat")));
	ensure(CharacterStat);

	ISLCharacterHUDInterface* HUDPawn = Cast<ISLCharacterHUDInterface>(GetOwningPlayerPawn());
	if (HUDPawn)
	{
		HUDPawn->SetupHUDWidget(this);
	}

}
