// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/SLGeneralData.h"
#include "SLProjectile.generated.h"

UCLASS(Abstract)
class SEKIROLIKE_API ASLProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASLProjectile();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

public:
	virtual void ResetProjectile();
	virtual void ActiveProjectile();
	virtual void FireInDirection(const FVector& ShootDirection, float InPower);

public:
	void InitializeProjectile(EProjectileType InProjectileType);

	FORCEINLINE EProjectileType GetProjectileType() { return MyProjectileType; }

	UFUNCTION()
	virtual void OnProjectileHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

protected:
	void ResetLifeTime();
	virtual void ReturnProjectile();

protected:
	EProjectileType MyProjectileType;

	UPROPERTY(VisibleAnywhere, Category = "Projectile")
	class USphereComponent* CollisionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	class UProjectileMovementComponent* ProjectileMovementComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Projectile")
	float ProjectileLifeTime;

	float CurrentLifeTime;

#if ENABLE_DRAW_DEBUG
	float DrawDebugAccum = 0.0f;
	float DrawDebugInterval = 0.1f;
#endif
};
