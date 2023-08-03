#include "AI/BTTask_PatrolMove.h"

EBTNodeResult::Type UBTTask_PatrolMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	int PatrolCount = Blackboard->GetValueAsInt(TEXT("PatrolCount"));
	++PatrolCount;
	Blackboard->SetValueAsInt(TEXT("PatrolCount"), PatrolCount);

	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	//UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	//AActor* TargetActor = Cast<AActor>(TargetObject);

	//if (nullptr == TargetActor)
	//{
	//	// ã�Ҵ� Ÿ���� ����.
	//	// ����
	//}
	//else if (nullptr != TargetActor)
	//{
	//	AActor* ResultActor = GetTargetSearch(OwnerComp);

	//	if (nullptr == ResultActor)
	//	{
	//		// ã�Ҵ� Ÿ���� ������ �����Լ� ���� �Ÿ� �̻� �־�����.
	//		// ����
	//	}
	//	else if (nullptr != ResultActor)
	//	{
	//		// ã�Ҵ� Ÿ���� �ְ� �����Լ� ���� �Ÿ� �ȿ� �ִ�.
	//		// ����
	//	}
	//}

	// ȸ��
	{
		FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector PatrolPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("PatrolPos"));

		CurPos.Z = 0.f;
		PatrolPos.Z = 0.f;

		FVector Dir = PatrolPos - CurPos;
		Dir.Normalize();

		FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (10.f <= FMath::Abs(Angle0 - Angle1))
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 500.f * DeltaSeconds });
			GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
		}
		else
		{
			FRotator Rot = Dir.Rotation();
			GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
		}
	}

	// �̵�
	{
		FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector PatrolPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("PatrolPos"));

		FVector Dis = PatrolPos - CurPos;

		GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(1);

		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));
		if (AttackRange >= Dis.Size())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

}
