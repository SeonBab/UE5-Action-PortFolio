#pragma once

#include "CoreMinimal.h"
#include "Engine/DataTable.h"
#include "SubClassUserWidgetData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FSubClassUserWidgetData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TSubclassOf<UUserWidget> UserWidget;
};
