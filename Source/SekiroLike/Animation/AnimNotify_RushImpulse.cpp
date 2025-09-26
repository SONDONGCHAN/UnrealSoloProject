// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RushImpulse.h"
#include "GameFramework/Character.h"
#include "Character/SLCharacterBase.h"

void UAnimNotify_RushImpulse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
    if (Owner)
    {
        FVector Forward = Owner->GetActorForwardVector();
        
        if (ASLCharacterBase* Character = Cast<ASLCharacterBase>(Owner))
        {
             Character->Dodge(Forward, StartSpeed, DodgeTime, MyCurveType);
        }
    }
}
