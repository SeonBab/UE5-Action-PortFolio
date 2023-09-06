#include "AI/Boss/Lich.h"
#include "AI/Boss/Enums_Boss.h"
#include "Global/GlobalGameInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Character/MainCharacter.h"

ALich::ALich()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTypeTag(TEXT("Monster"));
	SetAttackTypeTag(TEXT("MonsterAttack"));
	SetParrybool(false);

	DataName = "Lich";
	
	DarkBall = nullptr;
	FrostboltArray.Empty();
	
	MeleeDamage = 15.f;
	MaxPhase = 3;

	MeleeCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	MeleeCapsuleComponent->SetupAttachment(GetMesh(), TEXT("MeleeSocket"));
	MeleeCapsuleComponent->SetCollisionProfileName("NoCollision", true);
	MeleeCapsuleComponent->SetCapsuleHalfHeight(33.f);
	MeleeCapsuleComponent->SetCapsuleRadius(10.f);

	MeleeNiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	MeleeNiagaraComponent->SetupAttachment(MeleeCapsuleComponent);

	UCapsuleComponent* CurCapsuleComponent = GetCapsuleComponent();

	if (nullptr != CurCapsuleComponent)
	{
		FName ActorType = GetActorTypeTag();

		CurCapsuleComponent->SetCollisionProfileName(ActorType);
	}
}

FName ALich::GetDataName()
{
	return DataName;
}

void ALich::SetDarkBall(AActor* _Actor)
{
	DarkBall = _Actor;
}

AActor* ALich::GetDarkBall()
{
	return DarkBall;
}

UCapsuleComponent* ALich::GetMeleeCapsuleComponent()
{
	return MeleeCapsuleComponent;
}

UNiagaraComponent* ALich::GetMeleeNiagaraComponent()
{
	return MeleeNiagaraComponent;
}

TArray<AActor*>& ALich::GetFrostboltArray()
{
	return FrostboltArray;
}

void ALich::SetNullFrostboltArray(int _Index)
{
	FrostboltArray[_Index] = nullptr;
}

void ALich::Destroyed()
{
	if (DarkBall != nullptr || true == DarkBall->IsValidLowLevel())
	{
		DarkBall->Destroyed();
	}
	DarkBall = nullptr;

	for (int i = 0; i < FrostboltArray.Num(); i++)
	{
		if (nullptr != FrostboltArray[i] || true == FrostboltArray[i]->IsValidLowLevel())
		{
			FrostboltArray[i]->Destroy();
		}
	}
	FrostboltArray.Empty();

	Super::Destroyed();
}

void ALich::SetPhase(int _Phase)
{
	if (MaxPhase < _Phase)
	{
		return;
	}

	GetBlackboardComponent()->SetValueAsInt(TEXT("Phase"), _Phase);

	// ����� ���� ��Ƽ���� �ٲٱ�
}

int ALich::GetPhase()
{
	UBlackboardComponent* Blackboard = GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		return 0;
	}
	
	int CurPhase = Blackboard->GetValueAsInt(TEXT("Phase"));

	return CurPhase;
}

AActor* ALich::GetTargetActor()
{
	UBlackboardComponent* Blackboard = GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		return nullptr;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));

	if (nullptr == TargetObject)
	{
		return nullptr;
	}

	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (nullptr == TargetActor)
	{
		return nullptr;
	}

	return TargetActor;
}

void ALich::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurBossData = Inst->GetBossData(DataName);

		SetAllAnimation<BossAnimState>(CurBossData->MapAnimation);
		SetHP(CurBossData->HP);
		SetMaxHP(GetHP());
		SetAnimState(BossAnimState::Idle);
	}

	GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), this);
	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
	GetBlackboardComponent()->SetValueAsObject(TEXT("NavPath"), nullptr);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsDeath"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsReturn"), false);
	GetBlackboardComponent()->SetValueAsInt(TEXT("Phase"), 1);
	GetBlackboardComponent()->SetValueAsInt(TEXT("RandAttack"), 0);
	GetBlackboardComponent()->SetValueAsInt(TEXT("NavPathIndex"), 1);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("StateTime"), 0.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("MeleeAttackRange"), 300.f);
	GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);

	MeleeCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ALich::MeleeBeginOverlap);
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALich::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void ALich::MeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FPointDamageEvent DamageEvent;

	OtherActor->TakeDamage(MeleeDamage, DamageEvent, GetController(), this);
}

float ALich::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage�� ���� �޾Ҵ�.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (nullptr == EventInstigator)
		{
			return 0.f;
		}

		if (0.f < GetHP() && 0.f < FinalDamage)
		{
			// ü�� ����
			SetHP(GetHP() - FinalDamage);
			BpEventCallBossInfo();
		}

		if (0.f < GetHP())
		{
			// ����
			if (BossAnimState::Idle == GetAnimState<BossAnimState>() || BossAnimState::Walk == GetAnimState<BossAnimState>())
			{
				SetAnimState(BossAnimState::GotHit);
			}

			// ü�� 100���� ������ �����ϱ�
			if (100 < GetHP() && 200.f >= GetHP())
			{
				// 2������
				SetPhase(2);
			}
			else if (100.f >= GetHP())
			{
				// 3������
				SetPhase(3);
			}
		}
		else if (0.f >= GetHP())
		{
			// ü���� �������� �������� �ʾƾ��Ѵ�.
			SetHP(0.f);

			// ����
			SetAnimState(BossAnimState::Death);

			// ������ ���� �� ����
			UBlackboardComponent* Blackboard = GetBlackboardComponent();

			if (nullptr == Blackboard)
			{
				return 0.f;
			}

			Blackboard->SetValueAsBool(TEXT("IsDeath"), true);

			// �ݸ��� ����
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"), true);

			// �÷��̾��� ���� ����
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
		}

		return FinalDamage;
	}

	return FinalDamage;
}

