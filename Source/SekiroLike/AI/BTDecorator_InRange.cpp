// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTDecorator_InRange.h"
#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/SLAI.h"

UBTDecorator_InRange::UBTDecorator_InRange()
{
	NodeName = TEXT("InRange");
    bNotifyTick = true;
}

bool UBTDecorator_InRange::CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const
{
	return CheckCondition(OwnerComp);
}

void UBTDecorator_InRange::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	bool bResult = CheckCondition(OwnerComp);

	bResult = IsInversed() ? !bResult : bResult;

	if (!bResult)
	{
		OwnerComp.RequestExecution(this);
	}
}

bool UBTDecorator_InRange::CheckCondition(UBehaviorTreeComponent& OwnerComp) const
{
	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return false;
	}

	APawn* Target = Cast<APawn>(OwnerComp.GetBlackboardComponent()->GetValueAsObject(BBKEY_TARGET));
	if (nullptr == Target)
	{
		return false;
	}

	float DistanceToTarget = ControllingPawn->GetDistanceTo(Target);
	bool bResult = (DistanceToTarget <= Range);

	return bResult;
}