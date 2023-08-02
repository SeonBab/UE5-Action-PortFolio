#include "AI/BTTask_EquipWeapon.h"

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ResetStateTime(OwnerComp);

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	if (EWeaponType::UnArmed == CurWeaponType)
	{
		GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->SetLockOnCheck(true);

		if (500 <= Dir.Size())
		{
			GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetBow();
		}
		else
		{
			GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetSwordAndSheiled();
		}
	}

	return EBTNodeResult::Succeeded;
}
