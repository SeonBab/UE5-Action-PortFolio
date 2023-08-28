#include "AI/Boss/Lich.h"
#include "Global/GlobalGameInstance.h"
#include "AI/Boss/Enums_Boss.h"
#include "BehaviorTree/BlackboardComponent.h"

ALich::ALich()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTypeTag(TEXT("Monster"));
	SetAttackTypeTag(TEXT("MonsterAttack"));
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

	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);

}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALich::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

