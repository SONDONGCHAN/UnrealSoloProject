// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_ShootProjectile.h"
#include "Interface/SLAnimationAttackInterface.h"

void UAnimNotify_ShootProjectile::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp)
	{
		ISLAnimationAttackInterface* AttackPawm = Cast<ISLAnimationAttackInterface>(MeshComp->GetOwner());
		if (AttackPawm)
		{
			AttackPawm->ShootProjectile(ShootPower, MyProjectileType);
		}
	}
}
