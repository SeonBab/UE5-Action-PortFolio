// Fill out your copyright notice in the Description page of Project Settings.


#include "GlobalGameInstance.h"
#include "AnimationsData.h"
#include "WeaponData.h"

UGlobalGameInstance::UGlobalGameInstance()
{
	{
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/Data/DP_AnimationEnumsData.DP_AnimationEnumsData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			AllAnimations = DataTable.Object;
		}
	}

	{
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/Data/DT_WeaponData.DT_WeaponData'");
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

TMap<AnimPlayerState, class UAnimMontage*> UGlobalGameInstance::GetAllAnimations(FName Name)
{
	check(nullptr != AllAnimations)

	FAnimationsData* FindTable = AllAnimations->FindRow<FAnimationsData>(Name, Name.ToString());

	check(nullptr != FindTable)

	return FindTable->AllAnimations;
}


const FWeaponData* UGlobalGameInstance::GetWeaponData(FName Name)
{
	check(nullptr != WeaponDatas)

	FWeaponData* FindTable = WeaponDatas->FindRow<FWeaponData>(Name, Name.ToString());

	check(nullptr != FindTable)

	return FindTable;

}