#pragma once

#include "CoreMinimal.h"

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
	LockOnLeftRun,
	LockOnRight,
	LockOnRightRun,
	Attack,
	AimOrBlock,
	ParryorFire,
};