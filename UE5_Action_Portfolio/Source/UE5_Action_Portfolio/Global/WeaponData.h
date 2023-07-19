#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "WeaponData.generated.h"

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EWeaponType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	int Damage;

	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	// 활은 스켈레톤 매쉬 검은 스태틱 매쉬
	// 하나로 합쳐서 사용해야한다.
};
