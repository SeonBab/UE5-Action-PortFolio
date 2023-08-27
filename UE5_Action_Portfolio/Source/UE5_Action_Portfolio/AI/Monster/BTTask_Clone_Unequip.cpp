#include "AI/Monster/BTTask_Clone_Unequip.h"

EBTNodeResult::Type UBTTask_Clone_Unequip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	GetWeaponCharacter(OwnerComp)->bUseControllerRotationYaw = false;
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetUnArmed();
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(false);

	return EBTNodeResult::Succeeded;
}

void UBTTask_Clone_Unequip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
}
