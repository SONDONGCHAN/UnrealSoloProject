// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterBase.h"
#include "Interface/SLCharacterAIInterface.h"
#include "Interface/SLCharacterWidgetInterface.h"
#include "Interface/SLMonsterAttackCheckInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"     
#include "GameData/SLGeneralData.h"
#include "SLCharacterNonPlayer.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLCharacterNonPlayer : public ASLCharacterBase, public ISLCharacterAIInterface, public ISLCharacterWidgetInterface, public ISLMonsterAttackCheckInterface
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
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI(EAttackType InAttackType) override;

	FAICharacterAttackFinished OnAttackFinished;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

// UI Section
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Widget, Meta = (AllowPrivateAccess = "true"))
	TObjectPtr<class USLWidgetComponent> HpBar;

	virtual void SetupCharacterWidget(USLUserWidget* InUserWidget) override;

public:
	virtual FHitResult AttackHitCheckSphereSweep(FString AttackName) override;
	
protected:
	UBlackboardComponent* GetBlackboard() const
	{
		AAIController* AIController = Cast<AAIController>(GetController());
		return AIController ? AIController->GetBlackboardComponent() : nullptr;
	}
};
