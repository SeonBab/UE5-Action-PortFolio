#include "AI/BTTask_Return.h"

EBTNodeResult::Type UBTTask_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	Blackboard->SetValueAsInt(TEXT("PatrolCount"), 0);

	return EBTNodeResult::InProgress;
}

void UBTTask_Return::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	{
		FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector ReturnPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("SpawnPos"));

		CurPos.Z = 0.f;
		ReturnPos.Z = 0.f;

		FVector Dir = ReturnPos - CurPos;
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

	// ÀÌµ¿
	{
		FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
		FVector ReturnPos = GetBlackboardComponent(OwnerComp)->GetValueAsVector(TEXT("SpawnPos"));

		FVector Dis = ReturnPos - CurPos;

		GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(1);

		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));
		if (AttackRange >= Dis.Size())
		{
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}

}
