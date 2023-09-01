#include "AI/Boss/BTTask_Boss_Death.h"

EBTNodeResult::Type UBTTask_Boss_Death::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character)
    {
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Death);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Death::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}
