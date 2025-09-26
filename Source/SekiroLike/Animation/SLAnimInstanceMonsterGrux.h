// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SLAnimInstanceMonster.h"
#include "SLAnimInstanceMonsterGrux.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLAnimInstanceMonsterGrux : public USLAnimInstanceMonster
{
	GENERATED_BODY()
	
public:
	USLAnimInstanceMonsterGrux();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DelataTime) override;
};
