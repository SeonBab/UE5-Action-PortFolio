#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class TeamType : uint8
{
	Neutral,
	PlayerTeam,
	CloneTeam,
	MonsterTeam,
	BossTeam,
};

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	UnArmed,
	Bow,
	Sword,
	Shiled,
	Arrow,
};

UENUM(BlueprintType)
enum class CharacterAnimState : uint8
{
	None,
	Idle,
	Walk,
	WalkJump,
	Run,
	RunJump,
	Roll,
	EquipOrDisArmBow,
	EquipOrDisArmSwordAndShield,
	LockOnIdle,
	LockOnBackward,
	LockOnForward,
	LockOnForwardRun,
	LockOnLeft,
	LockOnRight,
	Attack,
	AimOrBlock,
	ParryorFire,
	GotHit,
	AimOrBlockGotHit,
	Death,
	Dizzy,
};

UENUM(BlueprintType)
enum class AIAnimState : uint8
{
	None,
	Idle,
	Walk,
	GotHit,
	Death,
	Attack,
};

UENUM(BlueprintType)
enum class BossAnimState : uint8
{
	None,
	Idle,
	Walk,
	GotHit,
	Death,
	Hide,
	Visible,
	Attack_Left,
	Attack_Right,
	Attack_SpawnTornado,
	Attack_FrostboltShot,
};