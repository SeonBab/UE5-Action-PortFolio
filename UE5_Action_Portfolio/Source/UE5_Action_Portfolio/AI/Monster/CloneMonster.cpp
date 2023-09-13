#include "AI/Monster/CloneMonster.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/MonsterData.h"
#include "Weapon/WeaponComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Character/MainCharacter.h"

ACloneMonster::ACloneMonster()
{
	SetAnimState<CharacterAnimState>(CharacterAnimState::Idle);

	SetActorTypeTag(TEXT("Monster"));
	SetAttackTypeTag(TEXT("MonsterAttack"));
	Tags.Add(GetActorTypeTag());
	SetParrybool(true);

	DataName = "CloneMonster";

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	WeaponComponent->SetMeshAttach(GetMesh());
}

UWeaponComponent* ACloneMonster::GetWeaponComponent()
{
	return WeaponComponent;
}

void ACloneMonster::BeginPlay()
{
	Super::BeginPlay();

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurMonsterData = Inst->GetMonsterData(DataName);
		SetHP(CurMonsterData->HP);
		SetMaxHP(GetHP());
	}

	GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), this);
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("StateTime"), 0.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("PatrolRange"), 800.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("MeleeAttackRange"), 180.f);
	GetBlackboardComponent()->SetValueAsInt(TEXT("PatrolCount"), 0);
	GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), GetActorLocation());
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsReturn"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsDeath"), false);

	this->bUseControllerRotationYaw = false;
}

void ACloneMonster::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

float ACloneMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage를 전달 받았다.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (nullptr == EventInstigator || false == EventInstigator->IsValidLowLevel())
		{
			return 0.f;
		}

		APawn* EventInstigatorPawn = EventInstigator->GetPawn();

		if (nullptr == EventInstigatorPawn || false == EventInstigatorPawn->IsValidLowLevel())
		{
			return 0.f;
		}

		bool IsRoll = WeaponComponent->GetIsRollMove();

		if (true == IsRoll)
		{
			return 0.f;
		}

		FVector HitDir = EventInstigatorPawn->GetActorLocation();
		HitDir.Z = 0;

		FVector CurPos = GetActorLocation();
		CurPos.Z = 0;

		FVector Dir = HitDir - CurPos;
		Dir.Normalize();

		FVector CurForward = GetActorForwardVector();
		CurForward.Normalize();

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = CurForward.Rotation().Yaw;

		bool BlockCheck = WeaponComponent->GetIsBlock();
		bool ParryCheck = WeaponComponent->GetIsParry();
		bool IsInvincibilityCheck = GetIsInvincibility();

		AGlobalCharacter* GlobalChar = Cast<AGlobalCharacter>(EventInstigatorPawn);

		if (nullptr == GlobalChar)
		{
			return 0.f;
		}

		bool EnemyParrybool = GlobalChar->GetParrybool();

		if (160.f >= FMath::Abs(Angle0 - Angle1) && true == BlockCheck)
		{
			FinalDamage *= 0.1f;
		}
		else if (true == IsInvincibilityCheck)
		{
			FinalDamage = 0.f;

			return FinalDamage;
		}
		else if (160.f >= FMath::Abs(Angle0 - Angle1) && (true == ParryCheck && true == EnemyParrybool))
		{
			FinalDamage = 0.f;

			AMainCharacter* EnemyChar = Cast<AMainCharacter>(GlobalChar);

			if (nullptr == EnemyChar)
			{
				return 0.f;
			}

			UWeaponComponent* EnemyWeaponComponent = EnemyChar->GetWeaponComponent();

			if (nullptr == EnemyWeaponComponent || false == EnemyWeaponComponent->IsValidLowLevel())
			{
				return 0.f;
			}

			EnemyWeaponComponent->ChangeNoCollision();

			EnemyChar->SetAnimState(CharacterAnimState::Dizzy);

			return FinalDamage;
		}

		if (0.f < GetHP() && 0.f < FinalDamage)
		{
			SetHP(GetHP() - FinalDamage);
		}

		if (0.f < GetHP())
		{
			// 생존
			WeaponComponent->GotHit(Dir);
		}
		else if (0.f >= GetHP())
		{
			// 체력은 음수값이 되지하지 않아야한다.
			SetHP(0.f);

			// 죽음
			GetBlackboardComponent()->SetValueAsBool(TEXT("IsDeath"), true);
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"), true);

			WeaponComponent->Death();

			bool PlayerCheck = EventInstigatorPawn->ActorHasTag("Player");

			if (true == PlayerCheck)
			{
				AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigatorPawn);

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