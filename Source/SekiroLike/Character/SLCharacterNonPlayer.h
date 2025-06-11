// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "Interface/SLCharacterAIInterface.h"
#include "SLCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLCharacterNonPlayer : public ASLCharacterBase, public ISLCharacterAIInterface
{
	GENERATED_BODY()
	
public:
	ASLCharacterNonPlayer();

protected:
	virtual void SetDeath() override;

//AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAIAttackRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI() override;


	FAICharacterAttackFinished OnAttackFinished;

	virtual void NotifyComboActionEnd() override;
};
