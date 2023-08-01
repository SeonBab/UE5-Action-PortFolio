// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "Engine/DataTable.h"
#include "Enums.h"
#include "GlobalGameInstance.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UGlobalGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	UGlobalGameInstance();
	~UGlobalGameInstance();

	USkeletalMesh* GetWeaponMesh(FName _Name);
	struct FWeaponData* GetWeaponData(FName _Name);
	struct FMonsterData* GetMonsterData(FName _Name);

	struct FAnimaitionData* GetAllAnimaitionDatas(FName _Name);

public:
	static FRandomStream MainRandom;

private:
	UPROPERTY()
	UDataTable* WeaponDatas;
	UPROPERTY()
	UDataTable* Animations;
	UPROPERTY()
	UDataTable* MonsterDatas;
};
