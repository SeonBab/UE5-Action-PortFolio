#include "Weapon/Arrow.h"
#include "Global/GlobalGameInstance.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	ArrowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));

	ArrowSkeletalMesh->SetupAttachment(RootComponent);
	ArrowSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	ArrowSkeletalMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Arrow")));

}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

