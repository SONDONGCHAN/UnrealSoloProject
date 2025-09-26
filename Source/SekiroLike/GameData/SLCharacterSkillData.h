
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SLCharacterSkillData.generated.h"

USTRUCT(BlueprintType)
struct FSLCharacterSkillData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSLCharacterSkillData() : AttackDamage(0.0f), AttackRange(0.0f), AttackRadius(0.0f), Mp(0.f) {}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
	float AttackRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = SkillData)
	float Mp;
};

