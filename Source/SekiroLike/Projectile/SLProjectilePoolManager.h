// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameData/SLGeneralData.h"
#include "SLProjectilePoolManager.generated.h"

class ASLProjectile;

UCLASS()
class SEKIROLIKE_API ASLProjectilePoolManager : public AActor
{
	GENERATED_BODY()
	
private:	
	ASLProjectilePoolManager();

public:
	~ASLProjectilePoolManager();

protected:
	virtual void BeginPlay() override;

public:
	static ASLProjectilePoolManager* GetInstance(UWorld* World);

	ASLProjectile* GetProjectile(EProjectileType ProjectileType);

	void ReturnProjectile(ASLProjectile* Projectile);

protected:
	UPROPERTY(EditAnywhere)
	TMap<EProjectileType, TSubclassOf<ASLProjectile>> ProjectileClasses;

	UPROPERTY(EditAnywhere)
	int32 BasePoolSize = 3;

private:
	static ASLProjectilePoolManager* MyInstance;

	TMap<EProjectileType, TQueue<ASLProjectile*>*> ProjectilePools;
};
