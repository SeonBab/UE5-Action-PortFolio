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
	Attack_SpawnTornado,
	Attack_MagicShot,
	GotHit,
	Death,
	Dizzy,
};