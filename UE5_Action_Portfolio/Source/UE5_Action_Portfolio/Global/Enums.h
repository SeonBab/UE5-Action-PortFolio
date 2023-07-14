#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	None,
	Unarm, // 무기 없음
	Bow, // 무기 없음
	Sword, // 무기 없음
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