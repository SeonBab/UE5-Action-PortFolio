#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Sound/SoundCue.h"
#include "Global/Enums.h"
#include "AnimaitionData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FAnimaitionData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EWeaponType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<CharacterAnimState, UAnimMontage*> AllAnimations;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<CharacterAnimState, USoundCue*> MapSoundCue;
};
