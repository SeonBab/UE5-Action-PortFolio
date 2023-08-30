#include "GlobalGameInstance.h"
#include "Global/Data/AnimaitionData.h"
#include "Global/Data/WeaponData.h"
#include "Global/Data/MonsterData.h"
#include "Global/Data/BossData.h"
#include "Global/Data/NiagaraData.h"
#include "Global/Data/SubClassData.h"

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
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/AI/Monster/DT_MonsterData.DT_MonsterData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			MonsterDatas = DataTable.Object;
		}
	}

	{
		// 보스 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/AI/BOSS/DT_BossData.DT_BossData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			BossDatas = DataTable.Object;
		}
	}

	{
		// 나이아가라 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_NiagaraData.DT_NiagaraData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			NiagaraDatas = DataTable.Object;
		}
	}

	{
		// 서브 클레스 데이터
		FString DataPath = TEXT("/Script/Engine.DataTable'/Game/BluePrint/Global/DT_SubClassData.DT_SubClassData'");
		ConstructorHelpers::FObjectFinder<UDataTable> DataTable(*DataPath);

		if (DataTable.Succeeded())
		{
			SubClassDatas = DataTable.Object;
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

	if (nullptr == FindMesh)
	{
		return nullptr;
	}

	return FindMesh;
}

UNiagaraSystem* UGlobalGameInstance::GetNiagaraAsset(FName _Name)
{
	if (nullptr == NiagaraDatas)
	{
		return nullptr;
	}

	FNiagaraData* FindTable = NiagaraDatas->FindRow<FNiagaraData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}

	UNiagaraSystem* FindNiagaraAsset = FindTable->NiagaraAsset;

	if (nullptr == FindNiagaraAsset)
	{
		return nullptr;
	}

	return FindNiagaraAsset;
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

FBossData* UGlobalGameInstance::GetBossData(FName _Name)
{
	if (nullptr == BossDatas)
	{
		return nullptr;
	}

	FBossData* FindTable = BossDatas->FindRow<FBossData>(_Name, _Name.ToString());

	if (nullptr == FindTable)
	{
		return nullptr;
	}

	return FindTable;
}

TSubclassOf<UObject> UGlobalGameInstance::GetSubClass(FName _Name)
{
	if (nullptr == SubClassDatas)
	{
		return nullptr;
	}

	FSubClassData* FindTable = SubClassDatas->FindRow<FSubClassData>(_Name, _Name.ToString());
	
	if (nullptr == FindTable)
	{
		return nullptr;
	}

	return FindTable->Object;
}

FAnimaitionData* UGlobalGameInstance::GetAllAnimaitionDatas(FName _Name)
{
	check(nullptr != Animations)

	FAnimaitionData* FindTable = Animations->FindRow<FAnimaitionData>(_Name, _Name.ToString());

	check(nullptr != FindTable)

	return FindTable;
}
