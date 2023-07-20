// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "AnimaitionData.h"
#include "WeaponData.h"

UGlobalGameInstance::UGlobalGameInstance()
{
	{
		// 에니메이션 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_AnimaitionData.DT_AnimaitionData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			AllAnimations = DataTable.Object;
		}
	}

	{
		// 무기 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_WeaponData.DT_WeaponData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			AllWeaponDatas = DataTable.Object;
		}
	}
}

UGlobalGameInstance::~UGlobalGameInstance()
{
}

FWeaponData* UGlobalGameInstance::GetAllWeaponDatas(FName _Name)
{
	check(nullptr != AllWeaponDatas)

	FWeaponData* FindTable = AllWeaponDatas->FindRow<FWeaponData>(_Name, _Name.ToString());
	
	check(nullptr != FindTable)

	return FindTable;
}

FAnimaitionData* UGlobalGameInstance::GetAllAnimaitionDatas(FName _Name)
{
	check(nullptr != AllAnimations)

	FAnimaitionData* FindTable = AllAnimations->FindRow<FAnimaitionData>(_Name, _Name.ToString());

	check(nullptr != FindTable)

	return FindTable;
}
