// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item/SLItemData.h"
#include "SLPotionItemData.generated.h"

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	Hp_Small = 0,
	Hp_Middle,
	Hp_Large
};

UCLASS()
class SEKIROLIKE_API USLPotionItemData : public USLItemData
{
	GENERATED_BODY()
	
public:
	FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("SLItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = PotionType)
	EPotionType PotionType;

//public:
//	UPROPERTY(EditAnywhere, Category = Weapon)
//	TSoftObjectPtr<USkeletalMesh> WeaponMesh;
	
};
