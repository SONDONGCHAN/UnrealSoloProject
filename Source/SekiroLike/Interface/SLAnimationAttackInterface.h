// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "GameData/SLGeneralData.h"
#include "SLAnimationAttackInterface.generated.h"

// This class does not need to be modified.

UINTERFACE(MinimalAPI)
class USLAnimationAttackInterface : public UInterface
{
	GENERATED_BODY()
};

class SEKIROLIKE_API ISLAnimationAttackInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void ComboAttackHitCheck() = 0;

	virtual void RushAttackHitCheck() = 0;

	virtual void ShadowStrikeHitCheck() = 0;

	virtual void ShootProjectile(float ShootPower, EProjectileType MyProjectileType) = 0;
};
