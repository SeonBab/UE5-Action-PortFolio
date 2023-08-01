// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_Idle.h"

EBTNodeResult::Type UBTTask_Idle::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	UObject* TargetObject = GetBlackboardComponent(OwnerComp)->GetValueAsObject(TEXT("TargetActor"));
	AActor* TargetActor = Cast<AActor>(TargetObject);

	if (nullptr != TargetActor)
	{
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::InProgress;
}

void UBTTask_Idle::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	AActor* ResultActor = GetTargetSearch(OwnerComp);

	if (nullptr != ResultActor)
	{
		GetBlackboardComponent(OwnerComp)->SetValueAsObject(TEXT("TargetActor"), ResultActor);
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
		return;
	}

	if (5.0f <= GetStateTime(OwnerComp))
	{
		ResetStateTime(OwnerComp);

		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}
