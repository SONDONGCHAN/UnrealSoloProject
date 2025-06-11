// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLHpBarWidget.h"
#include "Components/ProgressBar.h"
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

	ISLCharacterWidgetInterface* CharactereWidget = Cast<ISLCharacterWidgetInterface>(OwningActor);
	if (CharactereWidget)
	{
		CharactereWidget->SetupCharacterWidget(this);
	}
}

void USLHpBarWidget::UpdateHpBar(float NewCurrentHp)
{
	ensure(MaxHp > 0.f);
	if (HpProgressBar)
	{
		HpProgressBar->SetPercent(NewCurrentHp/ MaxHp);
	}
}
