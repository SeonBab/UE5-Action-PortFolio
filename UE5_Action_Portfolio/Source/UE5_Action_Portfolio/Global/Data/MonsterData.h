#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "MonsterData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FMonsterData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	float HP;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	class UBehaviorTree* AI;
};
