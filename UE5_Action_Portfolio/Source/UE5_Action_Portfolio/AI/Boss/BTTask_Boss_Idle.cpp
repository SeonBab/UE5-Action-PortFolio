#include "AI/Boss/BTTask_Boss_Idle.h"

EBTNodeResult::Type UBTTask_Boss_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->SetAnimState(BossAnimState::Idle);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (2.0f <= GetStateTime(OwnerComp))
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
