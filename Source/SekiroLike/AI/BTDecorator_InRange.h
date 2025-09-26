// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTDecorator.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BTDecorator_InRange.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API UBTDecorator_InRange : public UBTDecorator
{
	GENERATED_BODY()
	
public:
	UBTDecorator_InRange();

protected:
	virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	bool CheckCondition(UBehaviorTreeComponent& OwnerComp) const;

	UPROPERTY(EditAnywhere, Category = "Range")
	float Range;
};
