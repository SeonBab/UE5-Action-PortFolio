#include "AI/BTTask_AIBase.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "GameFramework/Character.h"

ACharacter* UBTTask_AIBase::GetCurCharacter(UBehaviorTreeComponent& OwnerComp)
{
	AAIController* AICon = OwnerComp.GetOwner<AAIController>();

	if (nullptr == AICon || false == AICon->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == AICon || false == AICon->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	ACharacter* Character = AICon->GetPawn<ACharacter>();

	if (nullptr == Character || false == Character->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Character || false == Character->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Character;
}

TArray<FVector> UBTTask_AIBase::PathFind(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor)
{
	return PathFind(_OwnerComp, _Actor->GetActorLocation());
}

TArray<FVector> UBTTask_AIBase::PathFind(UBehaviorTreeComponent& _OwnerComp, FVector _Pos)
{
	// 경로 저장
	UNavigationPath* PathObject = nullptr;

	// 경로 시작할 위치
	FVector StartPos = GetCurCharacter(_OwnerComp)->GetActorLocation();

	// 경로 탐색
	PathObject = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), StartPos, _Pos);

	// 경로가 잘 나왔는지 체크
	if (nullptr == PathObject || false == PathObject->IsValid())
	{
		return TArray<FVector>();
	}

	TArray<FVector> PathPoints = PathObject->PathPoints;

	return PathObject->PathPoints;
}

float UBTTask_AIBase::GetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if(nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	float StateTime = Blackboard->GetValueAsFloat(TEXT("StateTime"));

	return StateTime;
}

void UBTTask_AIBase::ResetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return;
	}

	Blackboard->SetValueAsFloat(TEXT("StateTime"), 0.f);
}

UBlackboardComponent* UBTTask_AIBase::GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Blackboard;
}

EBTNodeResult::Type UBTTask_AIBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Type();
}

void UBTTask_AIBase::OnGameplayTaskActivated(UGameplayTask& _Task)
{
}

void UBTTask_AIBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return;
	}

	float StateTime = Blackboard->GetValueAsFloat(TEXT("StateTime"));
	StateTime += DeltaSeconds;
	Blackboard->SetValueAsFloat(TEXT("StateTime"), StateTime);
}
