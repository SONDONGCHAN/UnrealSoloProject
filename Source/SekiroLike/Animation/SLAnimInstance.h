// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "SLAnimInstance.generated.h"

class ACharacte;
class UCharacterMovementComponent;

UCLASS()
class SEKIROLIKE_API USLAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	USLAnimInstance();
	
protected:
	virtual void NativeInitializeAnimation() override; // AnimInstance가 처음 생성될 때, 한 번 호출

	virtual void NativeUpdateAnimation(float DelataTime) override; // 프레임마다 호출

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<ACharacter> Owner;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character)
	TObjectPtr<UCharacterMovementComponent> Movement;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	FVector Velocity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float GroundSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsIdle : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float MovingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsFalling : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsJumping : 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float JumpingThreshould;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsSprinting : 1;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	float NormalSpeed = 450.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Character)
	uint8 bIsStealthMode : 1;

public:
	void SetStealthMode(bool isStealthMode) { bIsStealthMode = isStealthMode;}
};
