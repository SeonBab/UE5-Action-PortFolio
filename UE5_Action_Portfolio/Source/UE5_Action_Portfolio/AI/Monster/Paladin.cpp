#include "AI/Monster/Paladin.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/MonsterData.h"
#include "Weapon/WeaponAction.h"
#include "BehaviorTree/BlackboardComponent.h"

APaladin::APaladin()
{
	SetActorTypeTag(TEXT("Monster"));
	SetAttackTypeTag(TEXT("MonsterAttack"));
}

void APaladin::BeginPlay()
{
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	CurMonsterData = Inst->GetMonsterData(DataName);
	// 비헤이비어 트리 Set하기

	Super::BeginPlay();

	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("StateTime"), 0.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("PatrolRange"), 800.f);
	GetBlackboardComponent()->SetValueAsFloat(TEXT("AttackRange"), 180.f);
	GetBlackboardComponent()->SetValueAsInt(TEXT("PatrolCount"), 0);
	GetBlackboardComponent()->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	GetBlackboardComponent()->SetValueAsVector(TEXT("PatrolPos"), GetActorLocation());
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsReturn"), false);
	GetBlackboardComponent()->SetValueAsBool(TEXT("IsDeath"), false);
	//GetBlackboardComponent()->SetValueAsFloat(TEXT("HP"), CurMonsterData->HP);
	SetHP(CurMonsterData->HP);
	SetMaxHP(GetHP());

	this->bUseControllerRotationYaw = false;
}

void APaladin::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

float APaladin::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (0.f >= GetHP())
	{
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsDeath"), true);
	}


	return FinalDamage;
}