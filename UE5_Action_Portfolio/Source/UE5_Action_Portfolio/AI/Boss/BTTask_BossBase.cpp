#include "AI/Boss/BTTask_BossBase.h"

UBTTask_BossBase::UBTTask_BossBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_BossBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ResetStateTime(OwnerComp);

	return EBTNodeResult::InProgress;
}

void UBTTask_BossBase::OnGameplayTaskActivated(UGameplayTask&)
{

}

void UBTTask_BossBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (nullptr == Blackboard)
	{
		return;
	}

	bool IsDeath = Blackboard->GetValueAsBool(TEXT("IsDeath"));

	if (true == IsDeath)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
