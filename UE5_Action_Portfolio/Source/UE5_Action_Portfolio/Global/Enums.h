#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Unarm, // ���� ����
	Bow, // ���� ����
	Sword, // ���� ����
};


UENUM(BlueprintType)
enum class AnimPlayerState : uint8
{
	None,
	Idle,
	Walk,
	WalkJump,
	Run,
	RunJump,
};