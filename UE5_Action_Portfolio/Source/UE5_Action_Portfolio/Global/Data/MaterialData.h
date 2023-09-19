#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "Materials/Material.h"
#include "MaterialData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FMaterialData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	UMaterial* MaterialAsset;
};
