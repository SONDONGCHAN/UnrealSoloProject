// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/SLItemData.h"
#include "SLPowerBoosterItemData.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLPowerBoosterItemData : public USLItemData
{
	GENERATED_BODY()
	
public:
	USLPowerBoosterItemData();

public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("SLItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PowerBooster)
	float BoostAmount;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PowerBooster)
	float BoostTime;
};
