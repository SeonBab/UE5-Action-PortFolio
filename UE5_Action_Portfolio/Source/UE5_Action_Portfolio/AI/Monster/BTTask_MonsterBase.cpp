#include "AI/Monster/BTTask_MonsterBase.h"
#include "Kismet/GamePlayStatics.h"
#include "AI/Monster/AIPlayerCloneCon.h"

UBTTask_MonsterBase::UBTTask_MonsterBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

EBTNodeResult::Type UBTTask_MonsterBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ResetStateTime(OwnerComp);

	return EBTNodeResult::InProgress;
}

void UBTTask_MonsterBase::OnGameplayTaskActivated(UGameplayTask&)
{

}

void UBTTask_MonsterBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
