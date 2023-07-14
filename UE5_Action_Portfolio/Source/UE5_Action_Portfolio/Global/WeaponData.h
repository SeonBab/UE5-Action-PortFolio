// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include <Global/Enums.h>
#include "WeaponData.generated.h"

// Montage
class UAnimMontage;

USTRUCT(BlueprintType)
struct UE5_ACTION_PORTFOLIO_API FWeaponData : public FTableRowBase
{
	GENERATED_BODY()

		FWeaponData() {};
	~FWeaponData() {};

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	EWeaponType Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Data")
	TMap<AnimPlayerState, UAnimMontage*> AllAnimations;

};
