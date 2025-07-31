// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameData/SLGeneralData.h"
#include "AnimNotify_ShootProjectile.generated.h"

UCLASS()
class SEKIROLIKE_API UAnimNotify_ShootProjectile : public UAnimNotify
{
	GENERATED_BODY()
		
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	float ShootPower = 1000.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Projectile")
	EProjectileType MyProjectileType;
};
