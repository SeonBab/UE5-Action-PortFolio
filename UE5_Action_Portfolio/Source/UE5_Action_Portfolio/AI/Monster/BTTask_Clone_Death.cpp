#include "AI/Monster/BTTask_Clone_Death.h"

EBTNodeResult::Type UBTTask_Clone_Death::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Death::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
