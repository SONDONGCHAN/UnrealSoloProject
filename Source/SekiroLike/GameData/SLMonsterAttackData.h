
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SLMonsterAttackData.generated.h"

USTRUCT(BlueprintType)
struct FSLMonsterAttackData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSLMonsterAttackData() : AttackDamage(0.0f), AttackRange(0.0f), AttackRadius(0.0f), OffsetX(0.f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackData)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackData)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Attackata)
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = AttackData)
	float OffsetX;
};