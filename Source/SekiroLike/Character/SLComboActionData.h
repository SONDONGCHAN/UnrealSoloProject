// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "SLComboActionData.generated.h"

//USTRUCT(BlueprintType)
//struct FComboAnimInfo
//{
//	GENERATED_BODY()
//
//	UPROPERTY(EditAnywhere, Category = ComboData)
//	float EffectiveFrameCount;
//
//	UPROPERTY(EditAnywhere, Category = ComboData)
//	float AnimFPS;
//};

UCLASS()
class SEKIROLIKE_API USLComboActionData : public UDataAsset
{
	GENERATED_BODY()
	
public:
	USLComboActionData();

	UPROPERTY(EditAnywhere, Category = Name)
	FString MontageSectionNamePrefix;

	UPROPERTY(EditAnywhere, Category = Name)
	uint8 MaxComboCount;

	UPROPERTY(EditAnywhere, Category = Name)
	float FrameRate;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> EffectiveFrameCount;

	UPROPERTY(EditAnywhere, Category = ComboData)
	TArray<float> AnimFPS;
	
};
