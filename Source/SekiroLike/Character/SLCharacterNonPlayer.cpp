// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterNonPlayer.h"
#include "AI/SLAIController.h"
#include "Components/CapsuleComponent.h"

ASLCharacterNonPlayer::ASLCharacterNonPlayer()
{
	AIControllerClass = ASLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ASLCharacterNonPlayer::SetDeath()
{
	Super::SetDeath();

	FTimerHandle DeathTimerHandle;
	GetWorld()->GetTimerManager().SetTimer(DeathTimerHandle, FTimerDelegate::CreateLambda(
		[&]()
		{
			Destroy();
		}
	), DeathEventDelayTime, false);
}

float ASLCharacterNonPlayer::GetAIPatrolRadius()
{
	return 500.f;
}

float ASLCharacterNonPlayer::GetAIDetectRange()
{
	return 400.f;
}

float ASLCharacterNonPlayer::GetAIAttackRange()
{
	return GetCapsuleComponent()->GetScaledCapsuleRadius() + 50.f/*AttackRange*/ + (70.f * 2)/*(AttackRadius*2)*/;
}

float ASLCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.0f;
}

void ASLCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASLCharacterNonPlayer::AttackByAI()
{
	ProcessComboCommand();
}

void ASLCharacterNonPlayer::NotifyComboActionEnd()
{
	Super::NotifyComboActionEnd();
	OnAttackFinished.ExecuteIfBound();
}
