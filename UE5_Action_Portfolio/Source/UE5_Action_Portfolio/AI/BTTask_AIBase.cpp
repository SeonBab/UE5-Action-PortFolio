#include "AI/BTTask_AIBase.h"
#include "AIController.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "Global/GlobalCharacter.h"

UBTTask_AIBase::UBTTask_AIBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

AGlobalCharacter* UBTTask_AIBase::GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp)
{
	AAIController* AICon = OwnerComp.GetOwner<AAIController>();

	if (false == IsValid(AICon))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	AGlobalCharacter* Character = AICon->GetPawn<AGlobalCharacter>();

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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
	FVector StartPos = GetGlobalCharacter(_OwnerComp)->GetActorLocation();

	// 경로 탐색
	PathObject = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), StartPos, _Pos);

	// 경로가 잘 나왔는지 체크
	if (false == IsValid(PathObject))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return TArray<FVector>();
	}

	TArray<FVector> PathPoints = PathObject->PathPoints;

	return PathObject->PathPoints;
}

UNavigationPath* UBTTask_AIBase::PathFindNavPath(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor)
{
	return PathFindNavPath(_OwnerComp, _Actor->GetActorLocation());
}

UNavigationPath* UBTTask_AIBase::PathFindNavPath(UBehaviorTreeComponent& _OwnerComp, FVector _Pos)
{
	UNavigationPath* PathObject = nullptr;

	AGlobalCharacter* Character = GetGlobalCharacter(_OwnerComp);

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	FVector StartPos = Character->GetActorLocation();
	FVector EndPos = _Pos;

	PathObject = UNavigationSystemV1::FindPathToLocationSynchronously(GetWorld(), StartPos, EndPos);

	return PathObject;
}

float UBTTask_AIBase::GetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if(false == IsValid(Blackboard))"), __FUNCTION__, __LINE__);
		return 0.f;
	}

	float StateTime = Blackboard->GetValueAsFloat(TEXT("StateTime"));

	return StateTime;
}

void UBTTask_AIBase::ResetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (false == IsValid(Blackboard))"), __FUNCTION__, __LINE__);
		return;
	}

	Blackboard->SetValueAsFloat(TEXT("StateTime"), 0.f);
}

UBlackboardComponent* UBTTask_AIBase::GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (false == IsValid(Blackboard))"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Blackboard;
}

EBTNodeResult::Type UBTTask_AIBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ResetStateTime(OwnerComp);

	return EBTNodeResult::InProgress;
}

void UBTTask_AIBase::OnGameplayTaskActivated(UGameplayTask&)
{
}

void UBTTask_AIBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

	if (false == IsValid(Blackboard))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	bool IsDeath = Blackboard->GetValueAsBool(TEXT("IsDeath"));

	if (true == IsDeath)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	}

	float StateTime = Blackboard->GetValueAsFloat(TEXT("StateTime"));
	StateTime += DeltaSeconds;
	Blackboard->SetValueAsFloat(TEXT("StateTime"), StateTime);
}
