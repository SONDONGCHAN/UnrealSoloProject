// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_MoveToTarget.h"
#include "SLAI.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Interface/SLCharacterAIInterface.h"
#include "GameData/SLGeneralData.h"


UBTTask_MoveToTarget::UBTTask_MoveToTarget()
{
	bNotifyTick = false;
}

EBTNodeResult::Type UBTTask_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	EBTNodeResult::Type Result = Super::ExecuteTask(OwnerComp, NodeMemory);
	
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	ISLCharacterAIInterface* AIPawn = Cast<ISLCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return EBTNodeResult::Failed;
	}
	
	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return EBTNodeResult::Failed;
	}

	AAIController* AIController = Cast<AAIController>(ControllingPawn->GetController());
	if (AIController)
	{
		AIController->MoveToActor(Target);
	}
	
	return EBTNodeResult::InProgress;
}

void UBTTask_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	
}

EBTNodeResult::Type UBTTask_MoveToTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (ControllingPawn)
	{
		AAIController* AIController = Cast<AAIController>(ControllingPawn->GetController());
		if (AIController)
		{
			AIController->StopMovement();

		}
	}
	return EBTNodeResult::Aborted;
}
