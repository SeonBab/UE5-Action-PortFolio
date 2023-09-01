#include "AI/Boss/BTTask_Boss_Idle.h"

EBTNodeResult::Type UBTTask_Boss_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (nullptr == Character)
	{
		return EBTNodeResult::Failed;
	}

	Character->SetAnimState(BossAnimState::Idle);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (nullptr == Character)
	{
		return;
	}

	BossAnimState AnimState = Character->GetAnimState<BossAnimState>();

	if (2.0f <= GetStateTime(OwnerComp) && BossAnimState::GotHit != AnimState)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
