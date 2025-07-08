// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SLItemData.generated.h"

UENUM(BlueprintType)
enum class ETtemType : uint8
{
	Potion = 0,
	PowerBooster,
	StatEnhancer
};

UCLASS()
class SEKIROLIKE_API USLItemData : public UPrimaryDataAsset
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("SLItemData", GetFName());
	}
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = ItemType)
	ETtemType Type;
};
