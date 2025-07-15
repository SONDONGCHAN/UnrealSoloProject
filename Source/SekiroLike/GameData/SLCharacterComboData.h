
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SLCharacterComboData.generated.h"

USTRUCT(BlueprintType)
struct FSLCharacterComboData : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSLCharacterComboData() : AttackDamage(0.0f), AttackRange(0.0f), AttackRadius(0.0f){}

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRange;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float AttackRadius;

};