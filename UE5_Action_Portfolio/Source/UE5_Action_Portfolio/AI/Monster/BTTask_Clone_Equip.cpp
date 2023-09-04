#include "AI/Monster/BTTask_Clone_Equip.h"

EBTNodeResult::Type UBTTask_Clone_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetWeaponCharacter(OwnerComp)->bUseControllerRotationYaw = true;

	EWeaponType CurWeaponType = GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetWeaponCharacter(OwnerComp)->GetActorLocation();

	FVector Dir = TargetPos - CurPos;

	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->SetIsLockOn(true);

	if (600 <= Dir.Size())
	{
		if (EWeaponType::Bow != CurWeaponType)
		{
			GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetBow();
		}
	}
	else
	{
		if (EWeaponType::Sword != CurWeaponType)
		{
			GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->ChangeSetSwordAndSheiled();
		}
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	EWeaponType CurWeaponType = GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);
	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetWeaponCharacter(OwnerComp)->GetActorLocation();

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

	if (EWeaponType::UnArmed == CurWeaponType)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
