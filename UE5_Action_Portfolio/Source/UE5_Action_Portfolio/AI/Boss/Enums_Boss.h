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
	GotHit,
	Death,
	Dizzy,
};