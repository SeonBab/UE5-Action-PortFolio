#include "Weapon/WeaponComponent.h"
#include "Global/GlobalCharacter.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	CurWeaponAction = nullptr;

	UnArmedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UnArmedMesh"));
	BowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	SwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	ShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShiedlMesh"));

	BackBowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackBowMesh"));
	BackSwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackSwordMesh"));
	BackShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackShiedlMesh"));

	UnArmedWeaponMesh->SetGenerateOverlapEvents(true);
	BowWeaponMesh->SetGenerateOverlapEvents(true);
	SwordWeaponMesh->SetGenerateOverlapEvents(true);


}

void UWeaponComponent::SetMeshAttach(USkeletalMeshComponent* _Mesh)
{
	if (nullptr == _Mesh || false == _Mesh->IsValidLowLevel())
	{
		return;
	}

	BackBowWeaponMesh->SetupAttachment(_Mesh, TEXT("BackBow"));
	BackSwordWeaponMesh->SetupAttachment(_Mesh, TEXT("BackSword"));
	BackShieldWeaponMesh->SetupAttachment(_Mesh, TEXT("BackShield"));

	UnArmedWeaponMesh->SetupAttachment(_Mesh, TEXT("LeftHandSoket"));
	BowWeaponMesh->SetupAttachment(_Mesh, TEXT("LeftHandSoket"));
	SwordWeaponMesh->SetupAttachment(_Mesh, TEXT("RightHandSoket"));
	ShieldWeaponMesh->SetupAttachment(_Mesh, TEXT("LeftHandShield"));
}

UWeaponAction* UWeaponComponent::GetCurWeaponAction()
{
	return CurWeaponAction;
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
}


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

}

