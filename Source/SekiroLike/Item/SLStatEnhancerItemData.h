// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/SLItemData.h"
#include "GameData/SLCharacterStat.h"
#include "SLStatEnhancerItemData.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLStatEnhancerItemData : public USLItemData
{
	GENERATED_BODY()

public:
	USLStatEnhancerItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("SLItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = StatEnhancer)
	FSLCharacterStat ModifierStat;

};
