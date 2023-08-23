#pragma once

#include "CoreMinimal.h"

UENUM(BlueprintType)
enum class BossAnimState : uint8
{
	None,
	Idle,
	Walk,
	Attack_Left,
	Attack_Right,
	GotHit,
	Death,
	Dizzy,
};