// Fill out your copyright notice in the Description page of Project Settings.


#include "Curve/SLCurveManager.h"
#include "GameData/SLGeneralData.h"
#include "EngineUtils.h"

ASLCurveManager* ASLCurveManager::MyInstance = nullptr;

ASLCurveManager::ASLCurveManager()
{

}

void ASLCurveManager::BeginPlay()
{
	Super::BeginPlay();
	
}

ASLCurveManager* ASLCurveManager::GetInstance(UWorld* World)
{
	if (!MyInstance)
	{
		for (TActorIterator<ASLCurveManager> It(World); It; ++It)
		{
			MyInstance = *It;
			break;
		}
		if (!MyInstance)
		{
			MyInstance = World->SpawnActor<ASLCurveManager>();
		}
	}

	return MyInstance;
}

UCurveBase* ASLCurveManager::GetCurve(ECurveType CurveType)
{
	UCurveBase** CurvePtr = Curves.Find(CurveType);

	if (CurvePtr)
	{
		return *CurvePtr;
	}

	return nullptr;
}


