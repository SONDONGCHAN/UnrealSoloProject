// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SLAnimInstance.h"
#include "SLAnimInstanceMonster.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLAnimInstanceMonster : public USLAnimInstance
{
	GENERATED_BODY()
	
public:
	USLAnimInstanceMonster();

protected:
	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DelataTime) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Monster)
	float MovementDirection;
};
