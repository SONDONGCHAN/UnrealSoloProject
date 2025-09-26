
#pragma once

#include "CoreMinimal.h"
#include "SLGeneralData.generated.h"

UENUM()
enum class ECharacterType : uint8
{
	Player,
	Monster_Grux,
};

UENUM(BlueprintType)
enum class EProjectileType : uint8
{
	Player_Knife,
	MonsterA_Bullet,
};

UENUM(BlueprintType)
enum class ECurveType : uint8
{
	Downward_Curve,

};

UENUM(BlueprintType)
enum class ERangeType : uint8
{
	RangeConfront,
	RangeAttack,
};

UENUM(BlueprintType)
enum class EAttackType : uint8
{
	AttackNormal,
	Skill_A,
};