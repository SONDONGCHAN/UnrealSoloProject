// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTComposite_RandomSelector.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

//#include UE_INLINE_GENERATED_CPP_BY_NAME(BTComposite_RandomSelector)

UBTComposite_RandomSelector::UBTComposite_RandomSelector()
{
	NodeName = "Random Selector";
}

void UBTComposite_RandomSelector::InitializeMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryInit::Type InitType) const
{

}

void UBTComposite_RandomSelector::CleanupMemory(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTMemoryClear::Type ClearType) const
{

}

int32 UBTComposite_RandomSelector::GetNextChildHandler(FBehaviorTreeSearchData& SearchData, int32 PrevChild, EBTNodeResult::Type LastResult) const
{
    if (PrevChild == BTSpecialChild::NotInitialized || LastResult == EBTNodeResult::Failed)
    {
        const int32 NumChildren = GetChildrenNum();
        if (NumChildren > 0)
        {
            return FMath::RandRange(0, NumChildren - 1);
        }
    }

    return BTSpecialChild::ReturnToParent;

}
