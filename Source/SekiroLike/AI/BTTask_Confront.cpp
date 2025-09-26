// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Confront.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AI/SlAI.h"

UBTTask_Confront::UBTTask_Confront()
{
	NodeName = TEXT("Confront");
    bNotifyTick = true;
}

static FORCEINLINE FVector Flat(const FVector& V) { return FVector(V.X, V.Y, 0.f); }

EBTNodeResult::Type UBTTask_Confront::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        return EBTNodeResult::Failed;
    }
    
    ACharacter* AICharacter = Cast<ACharacter>(ControllingPawn);
    if (nullptr == AICharacter)
    {
        return EBTNodeResult::Failed;
    }
    
    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
    if (nullptr == Target)
    {
        return EBTNodeResult::Failed;
    }

    // 초기화
    FConfrontMemory* ConfrontData = reinterpret_cast<FConfrontMemory*>(NodeMemory);
    ConfrontData->Elapsed = 0.f;
    ConfrontData->TimeUntilChange = 0.f;
    ConfrontData->MoveDegree = 0;

    if (AICharacter->GetCharacterMovement())
    {
        AICharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
    }

    return EBTNodeResult::InProgress;
}

void UBTTask_Confront::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    ACharacter* AICharacter = Cast<ACharacter>(ControllingPawn);
    if (nullptr == AICharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
    APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
    if (nullptr == Target)
    {
        AICharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    FConfrontMemory* ConfrontData = reinterpret_cast<FConfrontMemory*>(NodeMemory);
    ConfrontData->Elapsed += DeltaSeconds;

    FVector ToTarget = Target->GetActorLocation() - AICharacter->GetActorLocation();
    float Distance = ToTarget.Size2D();
    ToTarget.Normalize();

    ConfrontData->TimeUntilChange -= DeltaSeconds;
    if (ConfrontData->TimeUntilChange <= 0.f)
    {
        ConfrontData->TimeUntilChange = DirChangeInterval;

        FVector2D Dir = FVector2D(ToTarget.X, ToTarget.Y).GetSafeNormal();

        if (Distance > DesiredRadius + RadiusTolerance)
        {
            int32 TargetDegree = FMath::RandRange(60, 90);
            if (!FMath::RandBool())
            {
                TargetDegree *= -1;
            }
            ConfrontData->MoveDegree = TargetDegree;
        }
        else if (Distance < DesiredRadius - RadiusTolerance)
        {
            int32 TargetDegree = FMath::RandRange(90, 120);
            if (!FMath::RandBool())
            {
                TargetDegree *= -1;
            }
            ConfrontData->MoveDegree = TargetDegree;
        }
        else
        {
            int32 TargetDegree = FMath::RandRange(60, 120);
            if (!FMath::RandBool())
            {
                TargetDegree *= -1;
            }
            ConfrontData->MoveDegree = TargetDegree;
        }
    }

    // 이동
    FVector2D MoveDir = { ToTarget.X, ToTarget.Y };
    MoveDir = RotateVector2D(MoveDir, ConfrontData->MoveDegree);
    FVector WorldDir(MoveDir.X, MoveDir.Y, 0.f);
    if (!WorldDir.IsNearlyZero())
    {
        WorldDir.Normalize();
        float Scale = MoveSpeed * DeltaSeconds;
        AICharacter->AddMovementInput(WorldDir, Scale);
    }
    
    // 시간이 다 되면 Task 종료
    if (ConfrontData->Elapsed >= Duration)
    {
        AICharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}

EBTNodeResult::Type UBTTask_Confront::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
    if (nullptr == ControllingPawn)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }

    ACharacter* AICharacter = Cast<ACharacter>(ControllingPawn);
    if (nullptr == AICharacter)
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
    }
    AICharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
    return EBTNodeResult::Aborted;
}

uint16 UBTTask_Confront::GetInstanceMemorySize() const
{
	return sizeof(FConfrontMemory);
}

FVector2D UBTTask_Confront::RotateVector2D(const FVector2D& V, float Degrees)
{
    float Rad = FMath::DegreesToRadians(Degrees);
    float Cos = FMath::Cos(Rad);
    float Sin = FMath::Sin(Rad);
    return FVector2D(V.X * Cos - V.Y * Sin, V.X * Sin + V.Y * Cos);
}
