// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SLTargetSystemComponent.generated.h"

DECLARE_MULTICAST_DELEGATE_OneParam(FOnTargetChangedDelegate, APawn* /*NewTarget*/);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SEKIROLIKE_API USLTargetSystemComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USLTargetSystemComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	bool DetectNearestTarget();
	TArray<APawn*> DetectNearByTargets();
	void ChangeTargetForMouse(FVector2D MouseInput);
	void RemoveTarget();
	FORCEINLINE APawn* GetCurrentTarget() { return CurrentTarget; }
	FORCEINLINE float GetMaxDistance() { return DetectDistance; }

private:
	bool LineTraceForTarget(APawn* InResut);
	void SetTarget(APawn* InTarget);
	APawn* CurrentTarget = nullptr;

protected:
	UPROPERTY(EditAnywhere, Category = "TargetSystem")
	float DetectRadius;

	UPROPERTY(EditAnywhere, Category = "TargetSystem")
	float DetectDistance;

	UPROPERTY(EditAnywhere, Category = "TargetSystem")
	float TargetSwitchCooldown = 0.4f;

	float LastTargetSwitchTime = -FLT_MAX;
	
public:
	FOnTargetChangedDelegate OnTargetChanged;

};
