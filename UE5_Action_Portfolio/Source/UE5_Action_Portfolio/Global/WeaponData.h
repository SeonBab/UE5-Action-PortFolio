// Fill out your copyright notice in the Description page of Project Settings.

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
	TMap<MainCharacterAnimState, UAnimMontage*> AllAnimations;
};
