// Fill out your copyright notice in the Description page of Project Settings.


#include "Character/SLCharacterNonPlayer.h"
#include "AI/SLAIController.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "UI/SLHpBarWidget.h"
#include "CharacterStat/SLCharacterStatComponent.h"
#include "UI/SLWidgetComponent.h"
#include "GameData/SLMonsterAttackData.h"
#include "GameData/SLGameSingleton.h"
#include "Physics/SLCollision.h"
#include "Engine/DamageEvents.h"

ASLCharacterNonPlayer::ASLCharacterNonPlayer()
{
	AIControllerClass = ASLAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
}

void ASLCharacterNonPlayer::SetDeath()
{
	Super::SetDeath();

	isDead = true;

	HpBar->SetHiddenInGame(true);
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
	return 400.f;
}

float ASLCharacterNonPlayer::GetAIDetectRange()
{
	return 500.f;
}

float ASLCharacterNonPlayer::GetAITurnSpeed()
{
	return 2.5f;
}

void ASLCharacterNonPlayer::SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished)
{
	OnAttackFinished = InOnAttackFinished;
}

void ASLCharacterNonPlayer::AttackByAI(EAttackType InAttackType)
{
	
}

float ASLCharacterNonPlayer::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Reuslt = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	return Reuslt;
}

void ASLCharacterNonPlayer::SetupCharacterWidget(USLUserWidget* InUserWidget)
{
	USLHpBarWidget* HpBarWidget = Cast<USLHpBarWidget>(InUserWidget);
	if (HpBarWidget)
	{
		HpBarWidget->UpdateStat(Stat->GetBaseStat(), Stat->GetModifierStat());
		HpBarWidget->UpdateCurrentHp(Stat->GetCurrentHp());
		Stat->OnStatChanged.AddUObject(HpBarWidget, &USLHpBarWidget::UpdateStat);
		Stat->OnHpChanged.AddUObject(HpBarWidget, &USLHpBarWidget::UpdateCurrentHp);
	}
}

FHitResult ASLCharacterNonPlayer::AttackHitCheckSphereSweep(FString AttackName)
{
	FHitResult OutHitResult;
	FCollisionQueryParams Params(SCENE_QUERY_STAT(Attack), false, this);

	FSLMonsterAttackData MonsterAttackData(USLGameSingleton::Get().GetMonsterAttackData(*AttackName));

	const float AttackRange = MonsterAttackData.AttackRange;
	const float AttackRadius = MonsterAttackData.AttackRadius;
	const float AttackDamage = MonsterAttackData.AttackDamage;
	const FVector Start = GetActorLocation() + (GetActorForwardVector() * MonsterAttackData.OffsetX);
	const FVector End = Start + GetActorForwardVector() * AttackRange;

	bool HitDetected = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, CCHANNEL_SLACTION, FCollisionShape::MakeSphere(AttackRadius), Params);
	if (HitDetected)
	{
		FDamageEvent DamageEvent;
		OutHitResult.GetActor()->TakeDamage(AttackDamage, DamageEvent, GetController(), this);
	}

#if ENABLE_DRAW_DEBUG

	FVector CapsuleOrigin = Start + (End - Start) * 0.5f;
	float CapsuleHalfHeight = (AttackRange * 0.5f) + AttackRadius;
	FColor DrawColor = HitDetected ? FColor::Red : FColor::Green;

	DrawDebugCapsule(GetWorld(), CapsuleOrigin, CapsuleHalfHeight, AttackRadius, FRotationMatrix::MakeFromZ(GetActorForwardVector()).ToQuat(), DrawColor, false, 2.0f);
#endif

	return OutHitResult;
}
