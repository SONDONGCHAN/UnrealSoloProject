// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLGameInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLGameInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SEKIROLIKE_API ISLGameInterface
{
	GENERATED_BODY()

public:
	virtual void OnPlayerScoreChanged(int32 NewPlayerScore) = 0;
	virtual void OnPlayerDead() = 0;
	virtual bool IsGameCleared() = 0;

};
