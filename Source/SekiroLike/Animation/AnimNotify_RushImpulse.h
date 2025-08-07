// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "GameData/SLGeneralData.h"
#include "AnimNotify_RushImpulse.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API UAnimNotify_RushImpulse : public UAnimNotify
{
	GENERATED_BODY()
	
protected:
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

public:
	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rush")
	//float RushPower = 1000.f; 

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rush")
	//bool bActive_Z = false;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Rush")
	//float ZPower= 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeDistance = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	float DodgeTime = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Dodge")
	ECurveType MyCurveType;

};
