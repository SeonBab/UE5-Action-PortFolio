// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Idle.h"

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->bUseControllerRotationYaw = false;

	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetUnArmed();
	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->SetLockOnCheck(false);
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
