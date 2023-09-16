#include "AI/Boss/BTTask_Boss_Death.h"

EBTNodeResult::Type UBTTask_Boss_Death::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character || false == Character->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Death);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Death::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}
