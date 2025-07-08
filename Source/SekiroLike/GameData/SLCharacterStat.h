
#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SLCharacterStat.generated.h"

USTRUCT(BlueprintType)
struct FSLCharacterStat : public FTableRowBase
{
	GENERATED_BODY()

public:
	FSLCharacterStat() : MaxHp(0.0f), MaxMp(0.0f), Attack(0.0f), MaxExp(0.0f){}
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxHp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxMp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float Attack;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Stat)
	float MaxExp;


	FSLCharacterStat operator+(const FSLCharacterStat& Other) const
	{
		const float* const ThisPtr = reinterpret_cast<const float* const>(this);
		const float* const OtherPtr = reinterpret_cast<const float* const>(&Other);

		FSLCharacterStat Result;
		float* ResultPtr = reinterpret_cast<float*>(&Result);
		int32 StatNum = sizeof(FSLCharacterStat) / sizeof(float);
		for (int32 i = 0; i < StatNum; i++)
		{
			ResultPtr[i] = ThisPtr[i] + OtherPtr[i];
		}

		return Result;
	}
};