// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/SLAnimInstancePlayer.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

USLAnimInstancePlayer::USLAnimInstancePlayer()
{
}

void USLAnimInstancePlayer::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void USLAnimInstancePlayer::NativeUpdateAnimation(float DelataTime)
{
	Super::NativeUpdateAnimation(DelataTime);

	if (Movement)
	{
		bIsSprinting = NormalSpeed < GroundSpeed;

		if (GroundSpeed > KINDA_SMALL_NUMBER)
		{
			FVector Velocity2D = FVector(Velocity.X, Velocity.Y, 0.f).GetSafeNormal();
			FRotator ControlRot = Owner->GetController() ? Owner->GetController()->GetControlRotation() : FRotator::ZeroRotator;
			FRotator YawRot(0.f, ControlRot.Yaw, 0.f);
			FVector LocalMoveDir = YawRot.UnrotateVector(Velocity2D);
			float ForwardDegrees = FMath::Atan2(LocalMoveDir.Y, LocalMoveDir.X) * (180.f / PI);
			MovementDirection = ForwardDegrees;
		}
		else
		{
			MovementDirection = 0.f;
		}
	}
}
