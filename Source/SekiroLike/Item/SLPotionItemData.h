// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/SLItemData.h"
#include "SLPotionItemData.generated.h"

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	Hp = 0,
	Mp
};

UCLASS()
class SEKIROLIKE_API USLPotionItemData : public USLItemData
{
	GENERATED_BODY()
	
public:
	USLPotionItemData();
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("SLItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Potion)
	EPotionType PotionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Potion)
	float HealAmount;
};
