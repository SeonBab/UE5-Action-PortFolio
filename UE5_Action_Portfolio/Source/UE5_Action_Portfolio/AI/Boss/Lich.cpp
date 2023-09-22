#include "AI/Boss/Lich.h"
#include "Global/Enums.h"
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
	Tags.Add(GetActorTypeTag());
	DataName = "Lich";

	SetParrybool(false);
	
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
	LostTarget();

	Super::Destroyed();
}

void ALich::SetPhase(int _Phase)
{
	if (MaxPhase < _Phase)
	{
		return;
	}

	GetBlackboardComponent()->SetValueAsInt(TEXT("Phase"), _Phase);
}

int ALich::GetPhase()
{
	UBlackboardComponent* Blackboard = GetBlackboardComponent();

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return 0;
	}
	
	int CurPhase = Blackboard->GetValueAsInt(TEXT("Phase"));

	return CurPhase;
}

AActor* ALich::GetTargetActor()
{
	UBlackboardComponent* Blackboard = GetBlackboardComponent();

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (false == IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return TargetActor;
}

void ALich::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurBossData = Instance->GetBossData(DataName);

	SetAllAnimation<BossAnimState>(CurBossData->MapAnimation);
	SetHP(CurBossData->HP);
	SetMaxHP(GetHP());
	SetAnimState(BossAnimState::Idle);

	UBlackboardComponent* CurBlackboardComponent = GetBlackboardComponent();

	if (false == IsValid(CurBlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurBlackboardComponent->SetValueAsObject(TEXT("SelfActor"), this);
	CurBlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
	CurBlackboardComponent->SetValueAsObject(TEXT("NavPath"), nullptr);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsDeath"), false);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsReturn"), false);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsBattle"), false);
	CurBlackboardComponent->SetValueAsInt(TEXT("Phase"), 1);
	CurBlackboardComponent->SetValueAsInt(TEXT("RandAttack"), 0);
	CurBlackboardComponent->SetValueAsInt(TEXT("NavPathIndex"), 1);
	CurBlackboardComponent->SetValueAsFloat(TEXT("StateTime"), 0.f);
	CurBlackboardComponent->SetValueAsFloat(TEXT("MeleeAttackRange"), 300.f);
	CurBlackboardComponent->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	CurBlackboardComponent->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);

	MeleeCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ALich::MeleeBeginOverlap);
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UBlackboardComponent* Blackboard = GetBlackboardComponent();

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));

	// 타겟을 잃었다면
	if (false == IsValid(TargetObject))
	{
		LostTarget();
		BpEventCallBossInfoOff();
	}
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

	// PointDamage를 전달 받았다.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (false == IsValid(EventInstigator))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		if (0.f < GetHP() && 0.f < FinalDamage)
		{
			// 체력 변경
			SetHP(GetHP() - FinalDamage);

			// 체력은 음수값이 되지하지 않아야한다.
			if (0.f >= GetHP())
			{
			SetHP(0.f);
			}

			// Info 표시
			BpEventCallBossInfoOn();
		}

		if (0.f < GetHP())
		{
			// 생존
			if (BossAnimState::Idle == GetAnimState<BossAnimState>() || BossAnimState::Walk == GetAnimState<BossAnimState>())
			{
				SetAnimState(BossAnimState::GotHit);
			}

			// 체력 1/3마다 페이즈 변경하기
			float CurMaxHP = GetMaxHP();
			float Phase2HP = CurMaxHP * 0.6f;
			float Phase3HP = CurMaxHP * 0.3f;

			if (Phase3HP < GetHP() && Phase2HP >= GetHP())
			{
				// 2페이즈
				SetPhase(2);
			}
			else if (Phase3HP >= GetHP())
			{
				// 3페이즈
				SetPhase(3);
			}
		}
		else if (0.f >= GetHP())
		{

			// 죽음
			SetAnimState(BossAnimState::Death);

			// 블랙보드 변수 값 변경
			UBlackboardComponent* Blackboard = GetBlackboardComponent();

			if (false == IsValid(Blackboard))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return 0.f;
			}

			Blackboard->SetValueAsBool(TEXT("IsDeath"), true);

			// 콜리전 변경
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"), true);

			// 플레이어의 락온 해제
			if (false == IsValid(EventInstigator))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return 0.f;
			}

			bool PlayerCheck = EventInstigator->GetPawn()->ActorHasTag("Player");

			if (true == PlayerCheck)
			{
				AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigator->GetPawn());

				if (nullptr == MainChar || false == MainChar->IsValidLowLevel())
				{
					UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
					return 0.f;
				}
			}
		}

		return FinalDamage;
	}

	return FinalDamage;
}

void ALich::LostTarget()
{
	if (DarkBall != nullptr)
	{
		DarkBall->Destroyed();
		DarkBall = nullptr;
	}

	for (int i = 0; i < FrostboltArray.Num(); i++)
	{
		if (nullptr != FrostboltArray[i])
		{
			FrostboltArray[i]->Destroy();
			FrostboltArray[i] = nullptr;
		}
	}
}

