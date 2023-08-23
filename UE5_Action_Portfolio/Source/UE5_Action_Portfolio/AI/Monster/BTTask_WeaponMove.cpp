#include "AI/Monster/BTTask_WeaponMove.h"

EBTNodeResult::Type UBTTask_WeaponMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	MoveTime = UGlobalGameInstance::MainRandom.FRandRange(1, 2);

    return EBTNodeResult::InProgress;
}

void UBTTask_WeaponMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();
	FVector PathPos;

	CurPos.Z = 0.f;
	
	// 무기 별 회전과 이동
	// 검
	if (EWeaponType::Sword == CurWeaponType)
	{
		TArray<FVector> PathPoint = PathFind(OwnerComp, TargetPos);

		if (0 != PathPoint.Num())
		{
			PathPos = PathPoint[1];
		}

		TargetPos.Z = 0.f;
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

		FVector Dis = TargetPos - CurPos;

		float AttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("AttackRange"));

		if (AttackRange < Dis.Size())
		{
			GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(1);
		}
		else if (AttackRange >= Dis.Size() && MoveTime <= GetStateTime(OwnerComp))
		{
			ResetStateTime(OwnerComp);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
		else
		{
			GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(0);
		}
	}
	// 활
	else if (EWeaponType::Bow == CurWeaponType)
	{
		// 회전

		TargetPos.Z = 0.f;
		PathPos.Z = 0.f;

		FVector Dir = TargetPos - CurPos;
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
		FVector Dis = TargetPos - CurPos;

		if (1000.f >= Dis.Size())
		{
			GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(-1);
		}
		else if (1500.f <= Dis.Size())
		{
			GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(1);
		}
		else
		{
			GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(0);
		}

		if (MoveTime <= GetStateTime(OwnerComp))
		{
			ResetStateTime(OwnerComp);
			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	// 무기 버그 실패
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
