#include "AI/Monster/BTTask_Unequip.h"

EBTNodeResult::Type UBTTask_Unequip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetGlobalCharacter(OwnerComp)->bUseControllerRotationYaw = false;
	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetUnArmed();
	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(false);

	return EBTNodeResult::Succeeded;
}

void UBTTask_Unequip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
