#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FNiagaraData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UNiagaraSystem* NiagaraAsset;
};
