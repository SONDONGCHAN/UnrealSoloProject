// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SLCharacterStat.h"
#include "SLGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSLGameSingleton, Error, All);

UCLASS()
class SEKIROLIKE_API USLGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	USLGameSingleton();

	static USLGameSingleton& Get();

//Character Stat Data Section
public:
	FORCEINLINE FSLCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel-1) ? CharacterStatTable[InLevel-1] : FSLCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FSLCharacterStat> CharacterStatTable;

};
