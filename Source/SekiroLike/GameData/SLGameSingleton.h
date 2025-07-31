// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SLCharacterStat.h"
#include "SLCharacterComboData.h"
#include "SLCharacterSkillData.h"
#include "SLGeneralData.h"
#include "SLGameSingleton.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSLGameSingleton, Error, All);

UCLASS()
class SEKIROLIKE_API USLGameSingleton : public UObject
{
	GENERATED_BODY()
	
public:
	USLGameSingleton();

	static USLGameSingleton& Get();

private:
	template<typename T>
	void SetDataArray(TArray<T>& InContainer, const TCHAR* InPath)
	{
		static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(InPath);
		if (nullptr != DataTableRef.Object)
		{
			const UDataTable* DataTable = DataTableRef.Object;
			check(DataTable->GetRowMap().Num() > 0);

			TArray<uint8*> ValueArray;
			DataTable->GetRowMap().GenerateValueArray(ValueArray);
			Algo::Transform(ValueArray, InContainer,
				[](uint8* Value)
				{
					return *reinterpret_cast<T*>(Value);
				}
			);
		}
	}

	void SetSkillDatas(const TCHAR* InPath);

//Character Stat Data Section
public:
	FORCEINLINE FSLCharacterStat GetCharacterStat(int32 InLevel) const { return CharacterStatTable.IsValidIndex(InLevel-1) ? CharacterStatTable[InLevel-1] : FSLCharacterStat(); }

	UPROPERTY()
	int32 CharacterMaxLevel;

private:
	TArray<FSLCharacterStat> CharacterStatTable;


// Character Combo Data Section
public:
	FSLCharacterComboData GetCharacterComboData(ECharacterType InCharacterType, int32 InCombo) const;

private:
	TMap<ECharacterType, TArray<FSLCharacterComboData>> CharacterComboDataTables;


// Character Skill Data Section
public:
	FORCEINLINE FSLCharacterSkillData GetCharacterSkillData(FName InSkillName) const
	{
		return CharacterSkillDatas.Contains(InSkillName) ? CharacterSkillDatas[InSkillName] : FSLCharacterSkillData();
	}

private:
	TMap<FName, FSLCharacterSkillData> CharacterSkillDatas;

};
