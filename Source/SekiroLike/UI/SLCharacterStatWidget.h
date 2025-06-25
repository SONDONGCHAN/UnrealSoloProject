// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameData/SLCharacterStat.h"
#include "SLCharacterStatWidget.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLCharacterStatWidget : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeConstruct() override;

public:
	void UpdateStat(const FSLCharacterStat& BaseStat, const FSLCharacterStat& ModifierStat);

private:
	UPROPERTY()
	TMap<FName, class UTextBlock*> BaseLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> ModifierLookup;

	UPROPERTY()
	TMap<FName, class UTextBlock*> NormalLookup;
	
};
