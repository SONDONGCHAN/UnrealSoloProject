// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile/SLProjectile.h"
#include "SLProjectilePlayerKnife.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API ASLProjectilePlayerKnife : public ASLProjectile
{
	GENERATED_BODY()
	
public:
	ASLProjectilePlayerKnife();

protected:
	virtual void BeginPlay() override;

public:
	virtual void Tick(float DeltaTime) override;

public:
	virtual void ResetProjectile() override;
	virtual void ActiveProjectile() override;
	virtual void FireInDirection(const FVector& ShootDirection, float InPower) override;

	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit) override;

protected:
	virtual void ReturnProjectile() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> FlyingEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> ThrowEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> HitPawnEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> HitEnvironmentEffect;

	UPROPERTY(VisibleAnywhere, Category = Effect)
	TObjectPtr<class UParticleSystemComponent> RemoveEffect;

	
};
