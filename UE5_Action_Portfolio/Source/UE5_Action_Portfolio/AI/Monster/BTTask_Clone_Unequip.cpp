#include "AI/Monster/BTTask_Clone_Unequip.h"

EBTNodeResult::Type UBTTask_Clone_Unequip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetWeaponCharacter(OwnerComp)->bUseControllerRotationYaw = false;
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(false);
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetUnArmed();

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Unequip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UWeaponAction* CurWeaponAction = GetWeaponCharacter(OwnerComp)->GetCurWeaponAction();

	if (CurWeaponAction == nullptr)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	EWeaponType CurWeaponType = CurWeaponAction->GetWeaponType();

	if (EWeaponType::UnArmed == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		CurWeaponAction->ChangeSetUnArmed();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
