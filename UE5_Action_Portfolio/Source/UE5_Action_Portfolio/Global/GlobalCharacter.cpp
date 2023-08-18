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

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
}

UWeaponAction* AGlobalCharacter::GetCurWeaponAction()
{
	return CurWeaponAction;
}

void AGlobalCharacter::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ���� �浹 ó��
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

FName AGlobalCharacter::GetActorTypeTag()
{
	return ActorTypeTag;
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

	// PointDamage�� ���� �޾Ҵ�.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (nullptr == EventInstigator)
		{
			return 0.f;
		}

		FVector HitDir = EventInstigator->GetPawn()->GetActorLocation();
		HitDir.Z = 0;

		FVector CurPos = GetActorLocation();
		CurPos.Z = 0;

		FVector Dir = HitDir - CurPos;
		Dir.Normalize();

		FVector CurForward = GetActorForwardVector();
		CurForward.Normalize();

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = CurForward.Rotation().Yaw;

		bool BlockCheck = GetCurWeaponAction()->GetIsBlock();
		bool ParryState = GetCurWeaponAction()->GetIsParry();

		if (160.f >= FMath::Abs(Angle0 - Angle1) && true == BlockCheck)
		{
			FinalDamage *= 0.1f;
		}
		else if (160.f >= FMath::Abs(Angle0 - Angle1) && true == ParryState)
		{
			FinalDamage = 0.f;

			AGlobalCharacter* EnemyChar = Cast<AGlobalCharacter>(EventInstigator->GetPawn());

			UWeaponAction* EnemyWeaponAction = EnemyChar->GetCurWeaponAction();

			EnemyWeaponAction->ChangeNoCollision();
			// �ִϸ��̼��� ���������� ��� �ȵ�
			EnemyWeaponAction->SetAnimState(CharacterAnimState::Dizzy);
			
			return FinalDamage;
		}

		if (0.f < HP && 0.f < FinalDamage)
		{
			HP -= FinalDamage;
		}
		
		if (0.f < HP)
		{
			GetCurWeaponAction()->GotHit(Dir);
		}
		else if (0.f >= HP)
		{
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"), true);

			GetCurWeaponAction()->Death();

			if (nullptr == EventInstigator)
			{
				return 0.f;
			}

			bool PlayerCheck = EventInstigator->GetPawn()->ActorHasTag("Player");
			
			if (true == PlayerCheck)
			{
				AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigator->GetPawn());

				if (nullptr != MainChar)
				{
					MainChar->LostLockedOnTargetActor();
				}
			}

			return FinalDamage;
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