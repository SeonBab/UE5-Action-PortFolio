#include "AI/BTTask_WeaponMove.h"

EBTNodeResult::Type UBTTask_WeaponMove::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	MoveTime = UGlobalGameInstance::MainRandom.FRandRange(1, 2);

    return EBTNodeResult::InProgress;
}

void UBTTask_WeaponMove::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	// 이동 시간이 길다면 실패
	// ex) 오래 따라다닌다면
	if (5.0f <= GetStateTime(OwnerComp))
	{
		ResetStateTime(OwnerComp);

		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	EWeaponType CurWeaponType = GetGlobalCharacter(OwnerComp)->GetCurWeaponAction()->GetWeaponType();

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	// 회전
	{
		FVector TargetPos = TargetActor->GetActorLocation();
		FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

		CurPos.Z = 0.f;
		TargetPos.Z = 0.f;

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
	}

	if (EWeaponType::Sword == CurWeaponType)
	{
		// 이동
		{
			FVector TargetPos = TargetActor->GetActorLocation();
			FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

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
	}
	else if (EWeaponType::Bow == CurWeaponType)
	{
		// 이동
		// 일정 거리 이상이면 랜덤으로 움직이기
		{
			FVector TargetPos = TargetActor->GetActorLocation();
			FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

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
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}
}
