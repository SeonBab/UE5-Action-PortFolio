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
			Animations = DataTable.Object;
		}
	}

	{
		// 무기 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_WeaponData.DT_WeaponData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			WeaponDatas = DataTable.Object;
		}
	}
}

UGlobalGameInstance::~UGlobalGameInstance()
{
}

FWeaponData* UGlobalGameInstance::GetWeaponData(FName _Name)
{
	check(nullptr != WeaponDatas)

	FWeaponData* FindTable = WeaponDatas->FindRow<FWeaponData>(_Name, _Name.ToString());
	
	check(nullptr != FindTable)

	return FindTable;
}

USkeletalMesh* UGlobalGameInstance::GetWeaponMesh(FName _Name)
{
	if (nullptr == WeaponDatas)
	{
		return nullptr;
	}

	FWeaponData* FindTable = WeaponDatas->FindRow<FWeaponData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}

	USkeletalMesh* FindMesh = FindTable->WeaponMesh;

	return FindMesh;
}

FAnimaitionData* UGlobalGameInstance::GetAllAnimaitionDatas(FName _Name)
{
	check(nullptr != Animations)

	FAnimaitionData* FindTable = Animations->FindRow<FAnimaitionData>(_Name, _Name.ToString());

	check(nullptr != FindTable)

	return FindTable;
}
