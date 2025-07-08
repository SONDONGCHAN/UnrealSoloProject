// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLCharacterStatWidget.h"
#include "Components/TextBlock.h"
#include "Components/HorizontalBox.h"

void USLCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FSLCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		/*  float MaxHp;
			float MaxMp;
			float Attack;
			float MaxExp; */
		
		const FName PropKey(PropIt->GetName());
		const FString PropName = PropIt->GetName();
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropName);
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropName);
		const FName TextBoostControlName = *FString::Printf(TEXT("Txt%sBoost"), *PropName);
		
		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}
		else
		{
			const FName TextNormalControlName = *FString::Printf(TEXT("Txt%s"), *PropName);
			UTextBlock* TextNormalBlock = Cast<UTextBlock>(GetWidgetFromName(TextNormalControlName));
			if (TextNormalBlock)
			{
				NormalLookup.Add(PropKey, TextNormalBlock);
			}

			continue;
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}

		UTextBlock* BoostTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBoostControlName));
		if (BoostTextBlock)
		{
			BoostLookup.Add(PropKey, BoostTextBlock);

			const FName TextBoostControlBoxName = *FString::Printf(TEXT("Txt%sBoostBox"), *PropName);
			UHorizontalBox* BoostTextBlockBox = Cast<UHorizontalBox>(GetWidgetFromName(TextBoostControlBoxName));
			if (BoostTextBlock)
			{
				BoostLookupBox.Add(PropKey, BoostTextBlockBox);
			}
		}
	}
}

void USLCharacterStatWidget::UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FSLCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
	
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			float BaseData = 0.0f;
			PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::FromInt(BaseData)));
		}
		else
		{
			UTextBlock** NormalTextBlockPtr = NormalLookup.Find(PropKey);
			if (NormalTextBlockPtr)
			{
				float BaseData = 0.0f;
				PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
				(*NormalTextBlockPtr)->SetText(FText::FromString(FString::FromInt(BaseData)));
			}
			continue;
		}
	
		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			float ModifierData = 0.0f;
			PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::FromInt(ModifierData)));
		}

	}
}

void USLCharacterStatWidget::UpdateBoostStat(const FSLCharacterStat& BoostStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FSLCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
	
		UTextBlock** BoostTextBlockPtr = BoostLookup.Find(PropKey);
		UHorizontalBox** BoostStatBox = BoostLookupBox.Find(PropKey);
		if (BoostTextBlockPtr)
		{
			float BoostData = 0.0f;
			PropIt->GetValue_InContainer((const void*)&BoostStat, &BoostData);
			(*BoostTextBlockPtr)->SetText(FText::FromString(FString::FromInt(BoostData)));

			if (BoostData <= KINDA_SMALL_NUMBER)
			{
				if (BoostStatBox)
				{
					(*BoostStatBox)->SetVisibility(ESlateVisibility::Collapsed);
				}
			}
			else
			{
				if (BoostStatBox)
				{
					(*BoostStatBox)->SetVisibility(ESlateVisibility::Visible);
				}
			}
		}
	}
}
