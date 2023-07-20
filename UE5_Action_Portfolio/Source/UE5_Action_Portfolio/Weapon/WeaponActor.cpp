#include "Weapon/WeaponActor.h"
#include "Global/GlobalGameInstance.h"
#include "Global/WeaponData.h"
#include "Global/Enums.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	SwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	BowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	ShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShiedlMesh"));

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}



	{
		struct FWeaponData* FindAnimaitionData = Instance->GetAllWeaponDatas(TEXT("UnArmed"));

		if (nullptr == FindAnimaitionData)
		{
			return;
		}

		USkeletalMesh* WeaponMesh = FindAnimaitionData->WeaponMesh;
		EWeaponType WeaponType = FindAnimaitionData->Type;

		if (nullptr == WeaponMesh || EWeaponType::None == WeaponType)
		{
			return;
		}

		WeaponArrays.Add(WeaponType, WeaponMesh);
	}

	{
		struct FWeaponData* FindAnimaitionData = Instance->GetAllWeaponDatas(TEXT("Bow"));

		if (nullptr == FindAnimaitionData)
		{
			return;
		}

		USkeletalMesh* WeaponMesh = FindAnimaitionData->WeaponMesh;
		EWeaponType WeaponType = FindAnimaitionData->Type;

		if (nullptr == WeaponMesh || EWeaponType::None == WeaponType)
		{
			return;
		}

		WeaponArrays.Add(WeaponType, WeaponMesh);
	}

	{
		struct FWeaponData* FindAnimaitionData = Instance->GetAllWeaponDatas(TEXT("SwordAndShield"));

		if (nullptr == FindAnimaitionData)
		{
			return;
		}

		USkeletalMesh* WeaponMesh = FindAnimaitionData->WeaponMesh;
		EWeaponType WeaponType = FindAnimaitionData->Type;

		if (nullptr == WeaponMesh || EWeaponType::None == WeaponType)
		{
			return;
		}

		WeaponArrays.Add(WeaponType, WeaponMesh);
	}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT(""));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(MeshLoader.Object);
	//	}
	//}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT(""));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(MeshLoader.Object);
	//	}
	//}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT(""));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(MeshLoader.Object);
	//	}
	//}
}


void AWeaponActor::BeginPlay()
{
	Super::BeginPlay();
	
}
