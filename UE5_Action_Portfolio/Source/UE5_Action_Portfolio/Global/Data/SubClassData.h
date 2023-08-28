#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SubClassData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FSubClassData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UObject> Object;
};
