// Fill out your copyright notice in the Description page of Project Settings.


#include "GameData/SLGameSingleton.h"

DEFINE_LOG_CATEGORY(LogSLGameSingleton);

USLGameSingleton::USLGameSingleton()
{
	static ConstructorHelpers::FObjectFinder<UDataTable> DataTableRef(TEXT("/Script/Engine.DataTable'/Game/SekiroLike/GameData/SLCharacterStatTable.SLCharacterStatTable'"));
	if (nullptr != DataTableRef.Object)
	{
		const UDataTable* DataTable = DataTableRef.Object;
		check(DataTable->GetRowMap().Num() > 0);
	
		TArray<uint8*> ValueArray;
		DataTable->GetRowMap().GenerateValueArray(ValueArray);
		Algo::Transform(ValueArray, CharacterStatTable,
			[](uint8* Value)
			{
				return *reinterpret_cast<FSLCharacterStat*>(Value);
			}
		);
	}
	
	CharacterMaxLevel = CharacterStatTable.Num();
	ensure(CharacterMaxLevel > 0);
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
