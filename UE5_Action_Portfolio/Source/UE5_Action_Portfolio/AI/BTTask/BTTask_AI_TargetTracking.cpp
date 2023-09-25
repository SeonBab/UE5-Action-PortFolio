#include "AI/BTTask/BTTask_AI_TargetTracking.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

EBTNodeResult::Type UBTTask_AI_TargetTracking::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return EBTNodeResult::Failed;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (false == IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return EBTNodeResult::Failed;
	}

	FVector TargetPos = TargetActor->GetActorLocation();

	UNavigationPath* NavPath = PathFindNavPath(OwnerComp, TargetPos);
	Blackboard->SetValueAsObject(TEXT("NavPath"), NavPath);
	Blackboard->SetValueAsVector(TEXT("NavPathLastPos"), TargetPos);

	return EBTNodeResult::InProgress;
}

void UBTTask_AI_TargetTracking::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (false == IsValid(TargetActor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = Character->GetActorLocation();
	FVector NavPathLastPos = Blackboard->GetValueAsVector(TEXT("NavPathLastPos"));

	// Ÿ�� ��ġ�� ���� �Ǿ���.
	if (TargetPos != NavPathLastPos)
	{
		UNavigationPath* NewNavPath = PathFindNavPath(OwnerComp, TargetPos);

		if (false == IsValid(NewNavPath))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return;
		}

		Blackboard->SetValueAsVector(TEXT("NavPathLastPos"), NavPathLastPos);
		Blackboard->SetValueAsObject(TEXT("NavPath"), NewNavPath);
		Blackboard->SetValueAsInt(TEXT("NavPathIndex"), 1);
	}

	UObject* NavObject = Blackboard->GetValueAsObject(TEXT("NavPath"));
	UNavigationPath* NavPath = Cast<UNavigationPath>(NavObject);

	if (false == IsValid(NavPath))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	if (true == NavPath->PathPoints.IsEmpty())
	{
		// ���� ����� ã�� ���ߴ�.
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> PathPoints.IsEmpty()"), __FUNCTION__, __LINE__);
		return;
	}

	// 0��°�� ������ ��ġ
	int NavPathIndex = 0;

	if (1 < NavPath->PathPoints.Num())
	{
		// ���������� ������ ��� NavPath�� 1���̹Ƿ� ���������� ������ ���� ��쿡�� �� ����
		NavPathIndex = Blackboard->GetValueAsInt(TEXT("NavPathIndex"));
	}

	FVector PathPos;

	PathPos = NavPath->PathPoints[NavPathIndex];


	TargetPos.Z = 0.f;
	CurPos.Z = 0.f;
	PathPos.Z = 0.f;

	// ȸ��
	{
		FVector Dir = PathPos - CurPos;
		Dir.Normalize();

		FVector OtherForward = Character->GetActorForwardVector();
		OtherForward.Normalize();

		FVector Cross = FVector::CrossProduct(OtherForward, Dir);

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (10.f <= FMath::Abs(Angle0 - Angle1))
		{
			FRotator Rot = FRotator::MakeFromEuler({ 0, 0, Cross.Z * 600.f * DeltaSeconds });
			Character->AddActorWorldRotation(Rot);
		}
		else
		{
			FRotator Rot = Dir.Rotation();
			Character->SetActorRotation(Rot);
		}
	}

	// �̵�
	{
		FVector Dir = PathPos - CurPos;

		Character->SetAnimState(AIAnimState::Walk);
		Character->AddMovementInput(Dir);

		if (10.f >= Dir.Size())
		{
			++NavPathIndex;
			Blackboard->SetValueAsInt(TEXT("NavPathIndex"), NavPathIndex);
		}

		FVector TargetDir = TargetPos - CurPos;

		float MeleeAttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("MeleeAttackRange"));

		if (MeleeAttackRange >= TargetDir.Size())
		{
			Blackboard->SetValueAsObject(TEXT("NavPath"), nullptr);
			Blackboard->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);
			Blackboard->SetValueAsInt(TEXT("NavPathIndex"), 1);

			FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		}
	}
	//Blackboard->SetValueAsBool(TEXT("IsReturn"), false);
}
