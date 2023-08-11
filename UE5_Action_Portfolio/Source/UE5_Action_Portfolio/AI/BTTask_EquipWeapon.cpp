#include "AI/BTTask_EquipWeapon.h"

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->bUseControllerRotationYaw = true;

	ResetStateTime(OwnerComp);

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(true);

	// 거리가 멀거나 가까워도 랜덤으로 변경 할지 안할지 만들자

	if (600 <= Dir.Size())
	{
		if (EWeaponType::Bow != CurWeaponType)
		{
			GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetBow();
		}
	}
	else
	{
		if (EWeaponType::Sword != CurWeaponType)
		{
			GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetSwordAndSheiled();
		}
	}

	return EBTNodeResult::Succeeded;
}
