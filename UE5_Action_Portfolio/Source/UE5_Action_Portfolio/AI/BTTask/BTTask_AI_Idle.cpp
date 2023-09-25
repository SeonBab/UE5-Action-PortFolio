#include "AI/BTTask/BTTask_AI_Idle.h"

EBTNodeResult::Type UBTTask_AI_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return EBTNodeResult::Failed;
	}

	Character->SetAnimState(AIAnimState::Idle);

	return EBTNodeResult::InProgress;
}

void UBTTask_AI_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	AIAnimState AnimState = Character->GetAnimState<AIAnimState>();

	if (2.0f <= GetStateTime(OwnerComp) && AIAnimState::GotHit != AnimState)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
