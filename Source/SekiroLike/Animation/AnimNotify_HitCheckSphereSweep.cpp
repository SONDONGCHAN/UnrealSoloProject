// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_HitCheckSphereSweep.h"
#include "Interface/SLMonsterAttackCheckInterface.h"

void UAnimNotify_HitCheckSphereSweep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ISLMonsterAttackCheckInterface* AttackPawm = Cast<ISLMonsterAttackCheckInterface>(MeshComp->GetOwner());
		if (AttackPawm)
		{
			AttackPawm->AttackHitCheckSphereSweep(AttackName);
		}
	}
}
