#include "AI/Monster/BTTask_MonsterBase.h"
#include "Kismet/GamePlayStatics.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

UBTTask_MonsterBase::UBTTask_MonsterBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

AGlobalCharacter* UBTTask_MonsterBase::GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp)
{
	AAICon* AICon = OwnerComp.GetOwner<AAICon>();

	if (nullptr == AICon || false == AICon->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == AICon || false == AICon->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}
	
	AGlobalCharacter* Character = AICon->GetPawn<AGlobalCharacter>();
	
	if (nullptr == Character || false == Character->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Character || false == Character->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Character;
}

float UBTTask_MonsterBase::GetStateTime(UBehaviorTreeComponent& OwnerComp)
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

void UBTTask_MonsterBase::ResetStateTime(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return;
	}

	Blackboard->SetValueAsFloat(TEXT("StateTime"), 0.f);
}

UBlackboardComponent* UBTTask_MonsterBase::GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Blackboard;
}

TArray<FVector> UBTTask_MonsterBase::PathFind(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor)
{
	return PathFind(_OwnerComp, _Actor->GetActorLocation());
}

TArray<FVector> UBTTask_MonsterBase::PathFind(UBehaviorTreeComponent& _OwnerComp, FVector _Pos)
{
	// 경로 저장
	UNavigationPath* PathObject = nullptr;
	// 경로 시작할 위치
	FVector StartPos = GetGlobalCharacter(_OwnerComp)->GetActorLocation();

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

EBTNodeResult::Type UBTTask_MonsterBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	ResetStateTime(OwnerComp);

	GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->DAndAButtonAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->RollorRunAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(0);

	return EBTNodeResult::InProgress;
}

void UBTTask_MonsterBase::OnGameplayTaskActivated(UGameplayTask& _Task)
{
	
}

void UBTTask_MonsterBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
