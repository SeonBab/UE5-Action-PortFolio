#include "AI/Boss/BTTask_Boss_Return.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

EBTNodeResult::Type UBTTask_Boss_Return::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (nullptr == Character)
	{
		return EBTNodeResult::Failed;
	}

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (nullptr == Blackboard)
	{
		return EBTNodeResult::Failed;
	}

	FVector ReturnPos = Blackboard->GetValueAsVector(TEXT("SpawnPos"));
	FVector CurPos = Character->GetActorLocation();

	UNavigationPath* NavPath = PathFindNavPath(OwnerComp, ReturnPos);
	Blackboard->SetValueAsObject(TEXT("NavPath"), NavPath);
	Blackboard->SetValueAsVector(TEXT("NavPathLastPos"), ReturnPos);

    return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Return::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (nullptr == Blackboard)
	{
		return;
	}

	AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

	if (nullptr == Character)
	{
		return;
	}

	FVector ReturnPos = Blackboard->GetValueAsVector(TEXT("SpawnPos"));
	FVector CurPos = Character->GetActorLocation();
	FVector NavPathLastPos = Blackboard->GetValueAsVector(TEXT("NavPathLastPos"));

	if (ReturnPos != NavPathLastPos)
	{
		UNavigationPath* NewNavPath = PathFindNavPath(OwnerComp, ReturnPos);

		if (nullptr == NewNavPath)
		{
			return;
		}

		Blackboard->SetValueAsObject(TEXT("NavPath"), NewNavPath);
	}

	UObject* NavObject = Blackboard->GetValueAsObject(TEXT("NavPath"));
	UNavigationPath* NavPath = Cast<UNavigationPath>(NavObject);

	if (nullptr == NavPath)
	{
		return;
	}

	if (true == NavPath->PathPoints.IsEmpty())
	{
		// 길을 제대로 찾지 못했다.
		return;
	}

	FVector PathPos;

	int NavPathIndex = Blackboard->GetValueAsInt(TEXT("NavPathIndex"));

	PathPos = NavPath->PathPoints[NavPathIndex];

	ReturnPos.Z = 0.f;
	CurPos.Z = 0.f;
	PathPos.Z = 0.f;

	// 회전
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

	// 이동
	FVector Dis = PathPos - CurPos;

	Character->SetAnimState(BossAnimState::Walk);
	Character->AddMovementInput(Dis);

	if (10.f >= Dis.Size())
	{
		++NavPathIndex;
		Blackboard->SetValueAsInt(TEXT("NavPathIndex"), NavPathIndex);
	}

	FVector ReturnDis = ReturnPos - CurPos;

	float MeleeAttackRange = GetBlackboardComponent(OwnerComp)->GetValueAsFloat(TEXT("MeleeAttackRange"));

	if (MeleeAttackRange >= ReturnDis.Size())
	{
		Blackboard->SetValueAsBool(TEXT("IsReturn"), false);
		Blackboard->SetValueAsObject(TEXT("NavPath"), nullptr);
		Blackboard->SetValueAsVector(TEXT("NavPathLastPos"), FVector::ZeroVector);
		Blackboard->SetValueAsInt(TEXT("NavPathIndex"), 1);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
