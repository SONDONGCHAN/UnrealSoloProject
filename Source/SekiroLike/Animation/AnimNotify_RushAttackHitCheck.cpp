// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RushAttackHitCheck.h"
#include "Interface/SLPlayerSkillCheckInterface.h"

void UAnimNotify_RushAttackHitCheck::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ISLPlayerSkillCheckInterface* AttackPawm = Cast<ISLPlayerSkillCheckInterface>(MeshComp->GetOwner());
		if (AttackPawm)
		{
			AttackPawm->RushAttackHitCheck();
		}
	}
}
