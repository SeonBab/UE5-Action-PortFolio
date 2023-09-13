#include "AI/Monster/BTTask_Clone_Return.h"

EBTNodeResult::Type UBTTask_Clone_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	WeaponComponent->SetIsLockOn(false);

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Return::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (nullptr == Blackboard)
	{
		return;
	}

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

	FVector ReturnPos = Blackboard->GetValueAsVector(TEXT("SpawnPos"));
	FVector CurPos = CloneMonster->GetActorLocation();
	FVector PathPos;

	TArray<FVector> PathPoint = PathFind(OwnerComp, ReturnPos);

	if (0 != PathPoint.Num())
	{
		PathPos = PathPoint[1];
	}

	ReturnPos.Z = 0.f;
	CurPos.Z = 0.f;
	PathPos.Z = 0.f;

	// ȸ��
	FVector Dir = PathPos - CurPos;
	Dir.Normalize();

	FVector OtherForward = CloneMonster->GetActorForwardVector();
	OtherForward.Normalize();

	FVector Cross = FVector::CrossProduct(OtherForward, Dir);

	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = OtherForward.Rotation().Yaw;

	if (10.f <= FMath::Abs(Angle0 - Angle1))
	{
		FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 600.f * DeltaSeconds });
		CloneMonster->AddActorWorldRotation(Rot);
	}
	else
	{
		FRotator Rot = Dir.Rotation();
		CloneMonster->SetActorRotation(Rot);
	}

	// �̵�
	WeaponComponent->WAndSButtonAction(1);

	FVector Dis = ReturnPos - CurPos;

	float MeleeAttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("MeleeAttackRange"));

	if (MeleeAttackRange >= Dis.Size())
	{
		Blackboard->SetValueAsInt(TEXT("PatrolCount"), 0);
		Blackboard->SetValueAsBool(TEXT("IsReturn"), false);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
