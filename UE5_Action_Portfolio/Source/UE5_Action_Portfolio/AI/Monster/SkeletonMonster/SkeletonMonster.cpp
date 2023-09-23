#include "AI/Monster/SkeletonMonster/SkeletonMonster.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/MonsterData.h"
#include "Global/Enums.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Engine/DamageEvents.h"


ASkeletonMonster::ASkeletonMonster()
{
    PrimaryActorTick.bCanEverTick = true;

    SetAnimState<MonsterAnimState>(MonsterAnimState::Idle);

    SetActorTypeTag(TEXT("Monster"));
    SetAttackTypeTag(TEXT("MonsterAttack"));
    Tags.Add(GetActorTypeTag());
    SetParrybool(false);

    DataName = "SkeletonMonster";

}

void ASkeletonMonster::BeginPlay()
{
    Super::BeginPlay();

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	const FMonsterData* MonsterData = Instance->GetMonsterData(DataName);

	if (nullptr == MonsterData)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == MonsterData"), __FUNCTION__, __LINE__);
		return;
	}

	SetHP(MonsterData->HP);
	SetMaxHP(GetHP());

	UBlackboardComponent* CurBlackboardComponent = GetBlackboardComponent();

	if (false == IsValid(CurBlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurBlackboardComponent->SetValueAsObject(TEXT("SelfActor"), this);
	CurBlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
	CurBlackboardComponent->SetValueAsObject(TEXT("NavPath"), nullptr);
	CurBlackboardComponent->SetValueAsFloat(TEXT("StateTime"), 0.f);
	CurBlackboardComponent->SetValueAsFloat(TEXT("MeleeAttackRange"), 100.f);
	CurBlackboardComponent->SetValueAsInt(TEXT("NavPathIndex"), 1);
	CurBlackboardComponent->SetValueAsVector(TEXT("SpawnPos"), GetActorLocation());
	CurBlackboardComponent->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsReturn"), false);
	CurBlackboardComponent->SetValueAsBool(TEXT("IsDeath"), false);

}

void ASkeletonMonster::Tick(float _DeltaTime)
{
    Super::Tick(_DeltaTime);
}

float ASkeletonMonster::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

    return FinalDamage;
}