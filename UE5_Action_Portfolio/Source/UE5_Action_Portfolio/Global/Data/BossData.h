#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Global/Enums.h"
#include "Sound/SoundCue.h"
#include "BossData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FBossData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBehaviorTree* AI;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<BossAnimState, UAnimMontage*> MapAnimation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<BossAnimState, USoundCue*> MapSoundCue;
};
