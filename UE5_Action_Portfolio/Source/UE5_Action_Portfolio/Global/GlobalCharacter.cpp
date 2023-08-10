#include "Global/GlobalCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Weapon/WeaponAction.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"

AGlobalCharacter::AGlobalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	UnArmedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UnArmedMesh"));
	BowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	SwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	ShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShiedlMesh"));

	UnArmedWeaponMesh->SetupAttachment(GetMesh(), TEXT("LeftHandSoket"));
	BowWeaponMesh->SetupAttachment(GetMesh(), TEXT("LeftHandSoket"));
	SwordWeaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandSoket"));
	ShieldWeaponMesh->SetupAttachment(GetMesh(), TEXT("LeftHandShield"));

	BackBowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackBowMesh"));
	BackSwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackSwordMesh"));
	BackShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackShiedlMesh"));

	BackBowWeaponMesh->SetupAttachment(GetMesh(), TEXT("BackBow"));
	BackSwordWeaponMesh->SetupAttachment(GetMesh(), TEXT("BackSword"));
	BackShieldWeaponMesh->SetupAttachment(GetMesh(), TEXT("BackShield"));
}

UWeaponAction* AGlobalCharacter::GetCurWeaponAction()
{
	return CurWeaponAction;
}

void AGlobalCharacter::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 공격 충돌 처리
}

FVector AGlobalCharacter::GetBowJointLocation()
{
	FVector Pos = BowWeaponMesh->GetSocketLocation(TEXT("Bow_04_Joint"));
	return Pos;
}

void AGlobalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurWeaponAction = NewObject<UWeaponAction>();
	CurWeaponAction->SetCurCharacter(this);

	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));

	BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
	BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
	BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));

	AnimState = CurWeaponAction->GetAnimState();

	UnArmedWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AGlobalCharacter::WeaponBeginOverlap);
	BowWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AGlobalCharacter::WeaponBeginOverlap);
	SwordWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AGlobalCharacter::WeaponBeginOverlap);
}

void AGlobalCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	CurWeaponAction->Tick(_DeltaTime);

	if (EWeaponType::Bow == CurWeaponAction->GetWeaponType())
	{
		BowChordMove();
	}
}

void AGlobalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AGlobalCharacter::BowChordMove()
{	
	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

	if (nullptr == BowAnim)
	{
		return;
	}

	if (true == BowAnim->GetBowChordCheck())
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

		if (nullptr != SkeletalMeshComponent)
		{
			FVector Vec = SkeletalMeshComponent->GetSocketLocation(TEXT("RightHandSoket"));

			BowAnim->SetHandTransform(Vec);
		}
	}
}