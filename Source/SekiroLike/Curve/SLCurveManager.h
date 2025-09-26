// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/SLGeneralData.h"
#include "SLCurveManager.generated.h"

UCLASS()
class SEKIROLIKE_API ASLCurveManager : public AActor
{
	GENERATED_BODY()
	
private:
	ASLCurveManager();

protected:
	virtual void BeginPlay() override;

public:
	static ASLCurveManager* GetInstance(UWorld* World);

	UCurveBase* GetCurve(ECurveType CurveType);


private:
	static ASLCurveManager* MyInstance;

protected:
	UPROPERTY(EditAnywhere)
	TMap<ECurveType, UCurveBase*> Curves;


};
