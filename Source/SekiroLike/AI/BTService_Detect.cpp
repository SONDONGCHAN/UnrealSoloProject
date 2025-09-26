// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTService_Detect.h"
#include "SLAI.h"
#include "AIController.h"
#include "Interface/SLCharacterAIInterface.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Physics/SLCollision.h"
#include "DrawDebugHelpers.h"
#include "Engine/OverlapResult.h"
#include "Character/SLCharacterBase.h"

UBTService_Detect::UBTService_Detect()
{
	NodeName = TEXT("Detect");
	Interval = 0.5f;
}

void UBTService_Detect::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	APawn* ControllingPawn = OwnerComp.GetAIOwner()->GetPawn();
	if (nullptr == ControllingPawn)
	{
		return;
	}

	UWorld* World = ControllingPawn->GetWorld();
	if (nullptr == World)
	{
		return;
	}

	ISLCharacterAIInterface* AIPawn = Cast<ISLCharacterAIInterface>(ControllingPawn);
	if (nullptr == AIPawn)
	{
		return;
	}
	float DetectRadius = AIPawn->GetAIDetectRange();
	FVector Center = ControllingPawn->GetActorLocation();

	TArray<FOverlapResult> OverlapResults;
	FCollisionQueryParams CollisionQueryParam(SCENE_QUERY_STAT(Detect), false, ControllingPawn);
	bool bResult = World->OverlapMultiByChannel(
		OverlapResults,
		Center,
		FQuat::Identity,
		CCHANNEL_SLACTION,
		FCollisionShape::MakeSphere(DetectRadius),
		CollisionQueryParam
	);

	if (bResult)
	{
		for (auto const& OverlapResult : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(OverlapResult.GetActor());
			if (Pawn && Pawn->GetController()->IsPlayerController())
			{
				//GetIsStealth
				if (!(Cast<ASLCharacterBase>(Pawn)->CanDetect()))
					break;

				OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, Pawn);

#if ENABLE_DRAW_DEBUG
				DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Red, false, 0.2f);

				DrawDebugPoint(World, Pawn->GetActorLocation(), 10.0f, FColor::Red, false, 0.2f);
				DrawDebugLine(World, ControllingPawn->GetActorLocation(), Pawn->GetActorLocation(), FColor::Red, false, 0.27f);
#endif
				return;
			}
		}
	}

	OwnerComp.GetBlackboardComponent()->SetValueAsObject(BBKEY_TARGET, nullptr);

#if ENABLE_DRAW_DEBUG
	DrawDebugSphere(World, Center, DetectRadius, 16, FColor::Green, false, 0.2f);
#endif
}
