
#pragma once

#include "CoreMinimal.h"
#include "SLGeneralData.generated.h"

UENUM()
enum class ECharacterType : uint8
{
	Player,
	Monster_A,
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Player_Knife,
	MonsterA_Bullet,
};