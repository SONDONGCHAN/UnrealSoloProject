// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/SLGameSingleton.h"

DEFINE_LOG_CATEGORY(LogSLGameSingleton);

USLGameSingleton::USLGameSingleton()
{
	SetDataArray<FSLCharacterStat>(CharacterStatTable, TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLCharacterStatTable.SLCharacterStatTable'"));
	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
	
	TArray<FSLCharacterComboData> CharacterComboDataTable;
	// Player Combo Data
	SetDataArray<FSLCharacterComboData>(CharacterComboDataTable, TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLPlayerComboDataTable.SLPlayerComboDataTable'"));
	CharacterComboDataTables.Emplace(ECharacterType::Player, CharacterComboDataTable);

	//  Skill Data
	SetSkillDatas(TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLCharacterSkillDatas.SLCharacterSkillDatas'"));
	SetMonsterAttackDatas(TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLMonsterAttackDatas.SLMonsterAttackDatas'"));
}

USLGameSingleton& USLGameSingleton::Get()
{
	USLGameSingleton* Singleton = CastChecked<USLGameSingleton>(GEngine->GameSingleton);
	if (Singleton)
	{
		return *Singleton;
	}

	UE_LOG(LogSLGameSingleton, Error, TEXT("Invalid Game Singleton"));
	return *NewObject<USLGameSingleton>();
}

void USLGameSingleton::SetSkillDatas(const TCHAR* InPath)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(InPath);
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);
		
		const TArray<FName> SkillNames = DataTable->GetRowNames();
		for (const FName& SkillName : SkillNames)
		{
			FSLCharacterSkillData* SkillData = DataTable->FindRow<FSLCharacterSkillData>(SkillName, TEXT("SkillData Load"));
			if (SkillData)
			{
				CharacterSkillDatas.Add(SkillName, *SkillData);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable not found at path: %s"), InPath);
	}
}

void USLGameSingleton::SetMonsterAttackDatas(const TCHAR* InPath)
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(InPath);
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);
	
		const TArray<FName> AttackNames = DataTable->GetRowNames();
		for (const FName& AttackName : AttackNames)
		{
			FSLMonsterAttackData* AttackData = DataTable->FindRow<FSLMonsterAttackData>(AttackName, TEXT("AttackData Load"));
			if (AttackData)
			{
				MonsterAttackDatas.Add(AttackName, *AttackData);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("DataTable not found at path: %s"), InPath);
	}
}

FSLCharacterComboData USLGameSingleton::GetCharacterComboData(ECharacterType InCharacterType, int32 InCombo) const
{
	if (CharacterComboDataTables.Contains(InCharacterType))
	{
		return CharacterComboDataTables[InCharacterType].IsValidIndex(InCombo - 1) ? 
			CharacterComboDataTables[InCharacterType][InCombo - 1] : FSLCharacterComboData();
	}
	
	return FSLCharacterComboData();
}