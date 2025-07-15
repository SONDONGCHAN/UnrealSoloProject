// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/SLGameSingleton.h"

DEFINE_LOG_CATEGORY(LogSLGameSingleton);

USLGameSingleton::USLGameSingleton()
{
	SetData<FSLCharacterStat>(CharacterStatTable, TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLCharacterStatTable.SLCharacterStatTable'"));
	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);

	TArray<FSLCharacterComboData> CharacterComboDataTable;
	SetData<FSLCharacterComboData>(CharacterComboDataTable, TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLCharacterComboDataTable.SLCharacterComboDataTable'"));
	CharacterComboDataTables.Emplace(ECharacterType::Player, CharacterComboDataTable);

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

FSLCharacterComboData USLGameSingleton::GetCharacterComboData(ECharacterType InCharacterType, int32 InCombo) const
{
	if (CharacterComboDataTables.Contains(InCharacterType))
	{
		return CharacterComboDataTables[InCharacterType].IsValidIndex(InCombo - 1) ? 
			CharacterComboDataTables[InCharacterType][InCombo - 1] : FSLCharacterComboData();
	}
	
	return FSLCharacterComboData();
}
