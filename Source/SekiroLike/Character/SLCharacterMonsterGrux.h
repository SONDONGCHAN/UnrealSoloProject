// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Character/SLCharacterNonPlayer.h"
#include "GameData/SLGeneralData.h"
#include "SLCharacterMonsterGrux.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLCharacterMonsterGrux : public ASLCharacterNonPlayer
{
	GENERATED_BODY()
	
public:
	ASLCharacterMonsterGrux();

protected:
	virtual void SetDeath() override;

	//AI Section
protected:
	virtual float GetAIPatrolRadius() override;
	virtual float GetAIDetectRange() override;
	virtual float GetAITurnSpeed() override;

	virtual void SetAIAttackDelegate(const FAICharacterAttackFinished& InOnAttackFinished) override;
	virtual void AttackByAI(EAttackType InAttackType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;

public:
	virtual FHitResult AttackHitCheckSphereSweep(FString AttackName) override;

	// UI Section
protected:
	virtual void SetupCharacterWidget(USLUserWidget* InUserWidget) override;

private:
	void NormalAttack();
	void NormalAttacknEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void RushAttack();
	void RushAttacknEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	void HitReact();
	void HitReactEnd(UAnimMontage* TargetMontage, bool IsProperlyEnded);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> NormalAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> RushAttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Animation)
	TObjectPtr<class UAnimMontage> HitReactMontage;

	// Hit React Section
protected:
	bool bIsHitReacting = false;

private:
	float NormalSpeed	= 340.f;

/*Effect*/
protected:
	/*Hit*/
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<UParticleSystemComponent> HitEffect;
};
