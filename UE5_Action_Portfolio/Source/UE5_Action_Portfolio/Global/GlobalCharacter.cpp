#include "Global/GlobalCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/WeaponData.h"
#include "Engine/DamageEvents.h"
#include "Weapon/WeaponAction.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"
#include "Character/MainCharacter.h"
#include "Components/CapsuleComponent.h"

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

	UnArmedWeaponMesh->SetGenerateOverlapEvents(true);
	BowWeaponMesh->SetGenerateOverlapEvents(true);
	SwordWeaponMesh->SetGenerateOverlapEvents(true);

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
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	EWeaponType CurWeaponEnum = GetCurWeaponAction()->GetWeaponType();
	FName WeaponName;

	const UEnum* WeaponEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	if (0 != WeaponEnum)
	{
		FString WeaponString = WeaponEnum->GetNameStringByValue((int64)CurWeaponEnum);
		WeaponName = FName(WeaponString);
	}

	FWeaponData* WeaponData = Instance->GetWeaponData(WeaponName);

	int CurWeaponDamage = WeaponData->Damage;
	FPointDamageEvent DamageEvent;
	//DamageEvent.HitInfo = ;

	OtherActor->TakeDamage(CurWeaponDamage, DamageEvent, GetController(), this);
}

FVector AGlobalCharacter::GetBowJointLocation()
{
	FVector Pos = BowWeaponMesh->GetSocketLocation(TEXT("Bow_04_Joint"));
	return Pos;
}

void AGlobalCharacter::SetActorTypeTag(FName _Tag)
{
	ActorTypeTag = _Tag;
}

void AGlobalCharacter::SetAttackTypeTag(FName _Tag)
{
	AttackTypeTag = _Tag;
}

FName AGlobalCharacter::GetAttackTypeTag()
{
	return AttackTypeTag;
}

void AGlobalCharacter::SetHP(float _HP)
{
	HP = _HP;
}

float AGlobalCharacter::GetHP()
{
	return HP;
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

	Tags.Add(ActorTypeTag);
	Tags.Add(AttackTypeTag);
	GetCurWeaponAction()->SetAttackType(AttackTypeTag);
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

float AGlobalCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage를 전달 받았다.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		FPointDamageEvent const* PointDamageEvent = (FPointDamageEvent*)(&DamageEvent);

		FVector HitDir = PointDamageEvent->ShotDirection;
		HitDir.Normalize();

		FVector CurPos = GetActorLocation();
		CurPos.Normalize();

		FVector Dir = HitDir - CurPos;
		Dir.Normalize();

		FVector CurForward = GetActorForwardVector();
		CurForward.Normalize();

		FVector Cross = FVector::CrossProduct(CurForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = CurForward.Rotation().Yaw;

		bool BlockCheck = GetCurWeaponAction()->GetIsBlock();

		if (160.f >= FMath::Abs(Angle0 - Angle1) && true == BlockCheck)
		{
			FinalDamage *= 0.1f;
		}


		if (0.f < HP && 0.f < FinalDamage)
		{
			HP -= FinalDamage;
		}
		
		if (0.f <= HP)
		{
			GetCurWeaponAction()->GotHit();
		}
		else if (0.f >= HP)
		{
			GetCurWeaponAction()->Death();

			

			bool PlayerCheck = EventInstigator->GetPawn()->ActorHasTag("Player");
			
			if (true == PlayerCheck)
			{
				AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigator->GetPawn());

				if (nullptr != MainChar)
				{
					MainChar->LostLockedOnTargetActor();
				}

			}

		}
	}
	
	return FinalDamage;
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