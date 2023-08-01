#include "GlobalGameInstance.h"
#include "Global/Data/AnimaitionData.h"
#include "Global/Data/WeaponData.h"
#include "Global/Data/MonsterData.h"

FRandomStream UGlobalGameInstance::MainRandom;

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

	{
		// 몬스터 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_MonsterData.DT_MonsterData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			MonsterDatas = DataTable.Object;
		}
	}
}

UGlobalGameInstance::~UGlobalGameInstance()
{
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

FWeaponData* UGlobalGameInstance::GetWeaponData(FName _Name)
{
	check(nullptr != WeaponDatas)

	FWeaponData* FindTable = WeaponDatas->FindRow<FWeaponData>(_Name, _Name.ToString());
	
	check(nullptr != FindTable)

	return FindTable;
}

FMonsterData* UGlobalGameInstance::GetMonsterData(FName _Name)
{
	if (nullptr == MonsterDatas)
	{
		return nullptr;
	}

	FMonsterData* FindTable = MonsterDatas->FindRow<FMonsterData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}

	return FindTable;
}

FAnimaitionData* UGlobalGameInstance::GetAllAnimaitionDatas(FName _Name)
{
	check(nullptr != Animations)

	FAnimaitionData* FindTable = Animations->FindRow<FAnimaitionData>(_Name, _Name.ToString());

	check(nullptr != FindTable)

	return FindTable;
}
