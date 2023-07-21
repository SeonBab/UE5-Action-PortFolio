#include "Weapon/WeaponActor.h"
#include "Global/GlobalGameInstance.h"
#include "Global/WeaponData.h"
#include "Global/Enums.h"

AWeaponActor::AWeaponActor()
{
	PrimaryActorTick.bCanEverTick = false;

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon/UnArmed_SkeletalMesh.UnArmed_SkeletalMesh'"));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(EWeaponType::UnArmed, MeshLoader.Object);
	//	}
	//}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon/SK_WP_Bow_04.SK_WP_Bow_04'"));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(EWeaponType::Bow, MeshLoader.Object);
	//	}
	//}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon/WP_Sword_04_A_WP_Sword_04_A.WP_Sword_04_A_WP_Sword_04_A'"));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(EWeaponType::Sword, MeshLoader.Object);
	//	}
	//}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT("/Script/Engine.SkeletalMesh'/Game/Weapon/WP_Sheild_03_A_WP_Sheild_03_A.WP_Sheild_03_A_WP_Sheild_03_A'"));

	//	if (true == MeshLoader.Succeeded())
	//	{
	//		WeaponArrays.Add(EWeaponType::Shiled, MeshLoader.Object);
	//	}
	//}

	//{
	//	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshLoader(TEXT("무기레퍼런스"));

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
