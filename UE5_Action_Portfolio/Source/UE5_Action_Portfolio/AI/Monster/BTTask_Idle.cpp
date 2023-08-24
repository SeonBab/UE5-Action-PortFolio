#include "AI/Monster/BTTask_Idle.h"

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->bUseControllerRotationYaw = false;
	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetUnArmed();
	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(false);
	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	if (5.0f <= GetStateTime(OwnerComp))
	{
		ResetStateTime(OwnerComp);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
