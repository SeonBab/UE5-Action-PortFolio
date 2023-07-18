#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	UnArmed,
	Bow,
	Sword,
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
	EquipArmBow,
	EquipArmSwordAndShield,
	DisArmBow,
	DisArmSwordAndShield,
	LockOnBackward,
	LockOnLeft,
	LockOnLeftRun,
	LockOnRight,
	LockOnRightRun,
	Attack,
	AimOrBlock,
	ShieldBlockIdle,
	Parry,
};