#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Particles/ParticleSystem.h"
#include "ParticleData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FParticleData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UParticleSystem* ParticleAsset;
};
