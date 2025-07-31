// Fill out your copyright notice in the Description page of Project Settings.


#include "Animation/AnimNotify_RushImpulse.h"
#include "GameFramework/Character.h"

void UAnimNotify_RushImpulse::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
    Super::Notify(MeshComp, Animation, EventReference);

    AActor* Owner = MeshComp ? MeshComp->GetOwner() : nullptr;
    if (Owner)
    {
        FVector Forward = Owner->GetActorForwardVector();

        if (ACharacter* Character = Cast<ACharacter>(Owner))
        {
            FVector LaunchDir = Forward * RushPower;
            if (bActive_Z)
            {
                FVector Up = Owner->GetActorUpVector();
                LaunchDir += (Up * ZPower);
            }

            Character->LaunchCharacter(LaunchDir, true, false);
        }
    }
}
