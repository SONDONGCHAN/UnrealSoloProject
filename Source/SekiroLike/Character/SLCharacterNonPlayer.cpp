// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterNonPlayer.h"
#include "AI/SLAIController.h"
#include "Components/CapsuleComponent.h"
//#include "CharacterStat/ABCharacterStatComponent.h"
//#include "Engine/AssetManager.h"

ASLCharacterNonPlayer::ASLCharacterNonPlayer()
{
	AIControllerClass = ASLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

}

void ASLCharacterNonPlayer::SetDeath()
{
	Super::SetDeath();

	ASLAIController* SLAIController = Cast<ASLAIController>(GetController());
	if (SLAIController)
	{
		SLAIController->StopAI();
	}

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
