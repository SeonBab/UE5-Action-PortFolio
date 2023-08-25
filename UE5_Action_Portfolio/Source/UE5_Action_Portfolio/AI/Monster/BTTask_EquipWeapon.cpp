#include "AI/Monster/BTTask_EquipWeapon.h"

EBTNodeResult::Type UBTTask_EquipWeapon::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetGlobalCharacter(OwnerComp)->bUseControllerRotationYaw = true;

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(true);

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

	return EBTNodeResult::InProgress;
}

void UBTTask_EquipWeapon::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	if (600 <= Dir.Size())
	{
		if (EWeaponType::Bow == CurWeaponType)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	else
	{
		if (EWeaponType::Sword == CurWeaponType)
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
}
