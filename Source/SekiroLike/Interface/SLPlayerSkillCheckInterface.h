// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "SLPlayerSkillCheckInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class USLPlayerSkillCheckInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class SEKIROLIKE_API ISLPlayerSkillCheckInterface
{
	GENERATED_BODY()

public:
	virtual void RushAttackHitCheck() = 0;

	virtual void ShadowStrikeHitCheck() = 0;

};
