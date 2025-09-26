// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTask_Confront.generated.h"

/**
 * 
 */
UCLASS()
class SEKIROLIKE_API UBTTask_Confront : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTask_Confront();

protected:
	// �ʼ� �������̵�
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	virtual uint16 GetInstanceMemorySize() const override;

	UPROPERTY(EditAnywhere, Category = "Confront")
	float Duration; // = 7.5f;
		
	UPROPERTY(EditAnywhere, Category = "Confront")
	float DirChangeInterval;  //= 2.5f; // �� �ʸ��� ���� �ٲ�

	UPROPERTY(EditAnywhere, Category = "Confront")
	float MoveSpeed; //= 20.f;

	// Ÿ���� �߽����� �����ϰ� ���� �� ������
	UPROPERTY(EditAnywhere, Category = "Confront")
	float DesiredRadius; //= 500.f;

	// ������ ��� ����
	UPROPERTY(EditAnywhere, Category = "Confront")
	float RadiusTolerance; //= 100.f;

private:
	FVector2D RotateVector2D(const FVector2D& V, float Degrees);

	struct FConfrontMemory
	{
		float Elapsed = 0.f;
		float TimeUntilChange = 0.f;
		int32 MoveDegree = 0;
		//FVector2D MoveDir = FVector2D::ZeroVector;
	};
};
