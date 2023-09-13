#include "AI/Monster/BTTask_Clone_Unequip.h"

EBTNodeResult::Type UBTTask_Clone_Unequip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

	if (nullptr == CloneMonster || false == CloneMonster->IsValidLowLevel())
	{
		return EBTNodeResult::Failed;
	}

	UWeaponComponent* WeaponComponent = CloneMonster->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return EBTNodeResult::Failed;
	}

	CloneMonster->bUseControllerRotationYaw = false;
	WeaponComponent->SetIsLockOn(false);
	WeaponComponent->ChangeSetUnArmed();

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Unequip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

	if (nullptr == CloneMonster || false == CloneMonster->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = CloneMonster->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();

	if (EWeaponType::UnArmed == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		WeaponComponent->ChangeSetUnArmed();
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
