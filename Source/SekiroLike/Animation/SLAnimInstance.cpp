// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SLAnimInstance.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


USLAnimInstance::USLAnimInstance()
{
	MovingThreshould = 3.f;
	JumpingThreshould = 100.f;
}

void USLAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();

	Owner = Cast<ACharacter>(GetOwningActor());
	if (Owner)
	{
		Movement = Owner->GetCharacterMovement();
	}
}

void USLAnimInstance::NativeUpdateAnimation(float DelataTime)
{
	Super::NativeUpdateAnimation(DelataTime);

	if (Movement)
	{
		Velocity = Movement->Velocity;
		GroundSpeed = Velocity.Size2D();
		bIsIdle = GroundSpeed < MovingThreshould;
		bIsFalling = Movement->IsFalling();
		bIsJumping = bIsFalling & (Velocity.Z > JumpingThreshould);
	}
}
