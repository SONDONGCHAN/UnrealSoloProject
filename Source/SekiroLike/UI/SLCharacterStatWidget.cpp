// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SLCharacterStatWidget.h"
#include "Components/TextBlock.h"

void USLCharacterStatWidget::NativeConstruct()
{
	Super::NativeConstruct();

	for (TFieldIterator<FNumericProperty> PropIt(FSLCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
		const FName TextBaseControlName = *FString::Printf(TEXT("Txt%sBase"), *PropIt->GetName());
		const FName TextModifierControlName = *FString::Printf(TEXT("Txt%sModifier"), *PropIt->GetName());

		UTextBlock* BaseTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextBaseControlName));
		if (BaseTextBlock)
		{
			BaseLookup.Add(PropKey, BaseTextBlock);
		}
		else
		{
			const FName TextControlName = *FString::Printf(TEXT("Txt%s"), *PropIt->GetName());
			UTextBlock* TextBlock = Cast<UTextBlock>(GetWidgetFromName(TextControlName));
			if (TextBlock)
			{
				NormalLookup.Add(PropKey, TextBlock);
			}

			continue;
		}

		UTextBlock* ModifierTextBlock = Cast<UTextBlock>(GetWidgetFromName(TextModifierControlName));
		if (ModifierTextBlock)
		{
			ModifierLookup.Add(PropKey, ModifierTextBlock);
		}
	}
}

void USLCharacterStatWidget::UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat)
{
	for (TFieldIterator<FNumericProperty> PropIt(FSLCharacterStat::StaticStruct()); PropIt; ++PropIt)
	{
		const FName PropKey(PropIt->GetName());
	
		float BaseData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&BaseStat, &BaseData);
		float ModifierData = 0.0f;
		PropIt->GetValue_InContainer((const void*)&ModifierStat, &ModifierData);
	
		UTextBlock** BaseTextBlockPtr = BaseLookup.Find(PropKey);
		if (BaseTextBlockPtr)
		{
			(*BaseTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
		}
		else
		{
			UTextBlock** NormalTextBlockPtr = NormalLookup.Find(PropKey);
			if (NormalTextBlockPtr)
			{
				(*NormalTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(BaseData)));
			}
			continue;
		}
	
		UTextBlock** ModifierTextBlockPtr = ModifierLookup.Find(PropKey);
		if (ModifierTextBlockPtr)
		{
			(*ModifierTextBlockPtr)->SetText(FText::FromString(FString::SanitizeFloat(ModifierData)));
		}
	}
}
