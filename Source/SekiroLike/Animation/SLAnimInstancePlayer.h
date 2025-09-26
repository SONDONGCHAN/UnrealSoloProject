// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/SLAnimInstance.h"
#include "SLAnimInstancePlayer.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API USLAnimInstancePlayer : public USLAnimInstance
{
	GENERATED_BODY()
	
public:
	USLAnimInstancePlayer();

protected:
	virtual void NativeInitializeAnimation() override; // AnimInstance가 처음 생성될 때, 한 번 호출

	virtual void NativeUpdateAnimation(float DelataTime) override; // 프레임마다 호출

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsSprinting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float NormalSpeed = 400.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsStealthMode : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsBlending : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovementDirection;

public:
	void SetStealthMode(bool isStealthMode) { bIsStealthMode = isStealthMode; }
	void SetBlending(bool isBlending) { bIsBlending = isBlending; }

	FORCEINLINE bool GetIsSprinting() { return bIsSprinting; }
};
