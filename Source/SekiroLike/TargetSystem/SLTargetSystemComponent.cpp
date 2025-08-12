// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetSystem/SLTargetSystemComponent.h"
#include "Engine/OverlapResult.h"

// Sets default values for this component's properties
USLTargetSystemComponent::USLTargetSystemComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void USLTargetSystemComponent::BeginPlay()
{
	Super::BeginPlay();
	
	DetectRadius = 1500.f;
	DetectDistance = DetectRadius + 200.f;
}

void USLTargetSystemComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	UWorld* World = GetWorld();
	if (!World || !CurrentTarget)
		return;

	if (!LineTraceForTarget(CurrentTarget))
	{
		DetectNearestTarget();
		//RemoveTarget();
	}
	else
	{
		FVector OwnerLocation = GetOwner()->GetActorLocation();
		FVector TargetLocation = CurrentTarget->GetActorLocation();

		float Dist = FVector::Dist(OwnerLocation, CurrentTarget->GetActorLocation());
		if (Dist > DetectDistance)
		{
			RemoveTarget();
		}
		else
		{
			DrawDebugPoint(World, TargetLocation, 10, FColor::Red, false, DeltaTime);
			DrawDebugLine(World, OwnerLocation, TargetLocation, FColor::Red, false, DeltaTime);
		}
	}
}

bool USLTargetSystemComponent::DetectNearestTarget()
{
	UWorld* World = GetWorld();
	if (!World) 
		return false;

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	TArray<FOverlapResult> OverlapResults;

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	bool bResult = World->OverlapMultiByObjectType(
		OverlapResults,
		OwnerLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		FCollisionShape::MakeSphere(DetectRadius),
		QueryParams
	);

	if (bResult)
	{
		float ClosestDistanceSq = FLT_MAX;
		APawn* ClosestEnemy = nullptr;

		for (const auto& Result : OverlapResults)
		{
			if (CurrentTarget && CurrentTarget == Result.GetActor())
				continue;

			APawn* Pawn = Cast<APawn>(Result.GetActor());
			if (Pawn && Pawn != GetOwner())
			{
				if (Pawn->ActorHasTag("Enemy"))
				{
					if (LineTraceForTarget(Cast<APawn>(Result.GetActor())))
					{
						float DistSq = FVector::DistSquared(Pawn->GetActorLocation(), OwnerLocation);
						if (DistSq < ClosestDistanceSq)
						{
							ClosestDistanceSq = DistSq;
							ClosestEnemy = Pawn;
						}
					}
				}
			}
		}
		
		if (ClosestEnemy)
		{
			SetTarget(ClosestEnemy);
			DrawDebugSphere(World, OwnerLocation, DetectRadius, 16, FColor::Red, false, 1.f);
			
			return true;
		}

	}

	DrawDebugSphere(World, OwnerLocation, DetectRadius, 16, FColor::Green, false, 1.0f);

	return false;
}

TArray<APawn*> USLTargetSystemComponent::DetectNearByTargets()
{
	TArray<APawn*> Result;

	UWorld* World = GetWorld();
	if (!World || !CurrentTarget) return Result;

	FVector OwnerLocation = GetOwner()->GetActorLocation();

	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	TArray<FOverlapResult> OverlapResults;

	bool bHasOverlap = World->OverlapMultiByObjectType(
		OverlapResults,
		OwnerLocation,
		FQuat::Identity,
		FCollisionObjectQueryParams(ECollisionChannel::ECC_Pawn),
		FCollisionShape::MakeSphere(DetectRadius),
		QueryParams
	);

	if (bHasOverlap)
	{
		for (const FOverlapResult& ResultItem : OverlapResults)
		{
			APawn* Pawn = Cast<APawn>(ResultItem.GetActor());
			if (Pawn && Pawn != GetOwner())
			{
				if (Pawn->ActorHasTag("Enemy"))
				{
					Result.Add(Pawn);
				}
			}
		}
	}

	return Result;
}

bool USLTargetSystemComponent::ChangeTargetForKey()
{
	return false;
}

void USLTargetSystemComponent::ChangeTargetForMouse(FVector2D MouseInput)
{
	const float InputThreshold = 4.0f;

	float CurrentTime = GetWorld()->GetTimeSeconds();

	if (CurrentTime - LastTargetSwitchTime < TargetSwitchCooldown)
		return;


	if (FMath::Abs(MouseInput.X) < InputThreshold)
		return;

	TArray<APawn*> CandidateTargets = DetectNearByTargets();

	if (CandidateTargets.Num() == 0)
		return;

	if (!CurrentTarget)
		return;

	FVector OwnerLocation = GetOwner()->GetActorLocation();
	FVector CurrentTargetLocation = CurrentTarget->GetActorLocation();
	FVector CurrentDirection = (CurrentTargetLocation - OwnerLocation).GetSafeNormal();

	FVector RightVector = FVector::CrossProduct(FVector::UpVector, CurrentDirection);

	bool bMoveRight = MouseInput.X > 0;

	APawn* BestCandidate = nullptr;
	float BestAngle = 180.0f;

	for (APawn* Candidate : CandidateTargets)
	{
		if (Candidate == CurrentTarget)
			continue;

		FVector CandidateDir = (Candidate->GetActorLocation() - OwnerLocation).GetSafeNormal();

		float SideDot = FVector::DotProduct(RightVector, CandidateDir);

		if ((bMoveRight && SideDot > 0) || (!bMoveRight && SideDot < 0))
		{
			float MaxAllowedAngle = 90.f;
			float AngleDiff = FMath::RadiansToDegrees(acosf(FVector::DotProduct(CurrentDirection, CandidateDir)));

			if (AngleDiff > MaxAllowedAngle)
				continue;

			if (AngleDiff < BestAngle)
			{
				BestAngle = AngleDiff;
				BestCandidate = Candidate;
			}
		}

		if (BestCandidate)
		{
			SetTarget(BestCandidate);
			LastTargetSwitchTime = CurrentTime;
		}
	}
}

bool USLTargetSystemComponent::LineTraceForTarget(APawn* InTarget)
{
	TArray<FHitResult> LineTraceResults;
	FCollisionQueryParams QueryParams;
	QueryParams.AddIgnoredActor(GetOwner());

	const FVector Start = GetOwner()->GetActorLocation();
	const FVector End = InTarget->GetActorLocation();


	bool isHit = GetWorld()->LineTraceMultiByChannel(LineTraceResults, Start, End, ECollisionChannel::ECC_Visibility, QueryParams);

	if (!isHit)
		return true;

	for (const FHitResult& Result : LineTraceResults)
	{
		AActor* HitActor = Result.GetActor();
		if (!HitActor) continue;

		if (HitActor == InTarget)
		{
			return true;
		}

		if (HitActor->IsA(APawn::StaticClass()) && HitActor != InTarget)
		{
			continue;
		}

		return false;
	}
	
	return true;
}

void USLTargetSystemComponent::SetTarget(APawn* InTarget)
{
	CurrentTarget = InTarget;
	PrimaryComponentTick.bCanEverTick = true;
	OnTargetChanged.Broadcast(CurrentTarget);
}

void USLTargetSystemComponent::RemoveTarget()
{
	CurrentTarget = nullptr;
	PrimaryComponentTick.bCanEverTick = false;
	OnTargetChanged.Broadcast(CurrentTarget);
}

