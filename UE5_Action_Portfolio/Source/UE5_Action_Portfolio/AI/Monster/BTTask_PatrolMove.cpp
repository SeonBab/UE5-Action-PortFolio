#include "AI/Monster/BTTask_PatrolMove.h"

EBTNodeResult::Type UBTTask_PatrolMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::InProgress;
}

void UBTTask_PatrolMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	FVector PatrolPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("PatrolPos"));
	FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	FVector PathPos;

	TArray<FVector> PathPoint = PathFind(OwnerComp, PatrolPos);

	if (0 != PathPoint.Num())
	{
		PathPos = PathPoint[1];
	}

	PatrolPos.Z = 0.f;
	CurPos.Z = 0.f;
	PathPos.Z = 0.f;

	// 회전
	FVector Dir = PathPos - CurPos;
	Dir.Normalize();

	FVector OtherForward = GetGlobalCharacter(OwnerComp)->GetActorForwardVector();
	OtherForward.Normalize();

	FVector Cross = FVector::CrossProduct(OtherForward, Dir);

	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = OtherForward.Rotation().Yaw;

	if (10.f <= FMath::Abs(Angle0 - Angle1))
	{
		FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 600.f * DeltaSeconds });
		GetGlobalCharacter(OwnerComp)->AddActorWorldRotation(Rot);
	}
	else
	{
		FRotator Rot = Dir.Rotation();
		GetGlobalCharacter(OwnerComp)->SetActorRotation(Rot);
	}

	// 이동
	FVector Dis = PatrolPos - CurPos;

	GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(1);

	float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

	if (AttackRange >= Dis.Size())
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
