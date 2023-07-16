// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "WeaponData.h"

UGlobalGameInstance::UGlobalGameInstance()
{
	FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_WeaponData.DT_WeaponData'");
	ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

	if (DataTable.Succeeded())
	{
		WeaponDatas = DataTable.Object;
	}
}

UGlobalGameInstance::~UGlobalGameInstance()
{
}

TMap<MainCharacterAnimState, UAnimMontage*> UGlobalGameInstance::GetAllAnimations(FName Name)
{
	check(nullptr != AllAnimations)

	FWeaponData* FindTable = AllAnimations->FindRow<FWeaponData>(Name, Name.ToString());

	check(nullptr != FindTable)

	return FindTable->AllAnimations;
}

FWeaponData* UGlobalGameInstance::GetWeaponData(FName Name)
{
	check(nullptr != WeaponDatas)

	FWeaponData* FindTable = WeaponDatas->FindRow<FWeaponData>(Name, Name.ToString());

	check(nullptr != FindTable)

	return FindTable;
}
