// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Composites/BTComposite_Selector.h"
#include "BTComposite_RandomSelector.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API UBTComposite_RandomSelector : public UBTComposite_Selector
{
	GENERATED_BODY()

public:
	UBTComposite_RandomSelector();

	virtual void InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const override;
	virtual void CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type ClearType) const override;

protected:
    virtual int32 GetNextChildHandler(struct FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const override;

};
