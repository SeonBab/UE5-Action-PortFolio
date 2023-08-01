// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AIBase.h"
#include "Kismet/GamePlayStatics.h"

UBTTask_AIBase::UBTTask_AIBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

AGlobalCharacter* UBTTask_AIBase::GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp)
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

void UBTTask_AIBase::SetBTTaskChange(UBehaviorTreeComponent& OwnerComp, AIBTTask _Task)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return;
	}

	Blackboard->SetValueAsEnum(TEXT("AIBTTask"), static_cast<uint8>(_Task));

	ResetStateTime(OwnerComp);

	FinishLatentTask(OwnerComp, EBTNodeResult::Type::Failed);
}

AActor* UBTTask_AIBase::GetTargetSearch(UBehaviorTreeComponent& OwnerComp)
{
	UBlackboardComponent* Blackboard = OwnerComp.GetBlackboardComponent();

	if (nullptr == Blackboard)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Blackboard)"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	FString TargetTag = Blackboard->GetValueAsString(TEXT("TargetTag"));

	TArray<AActor*> TargetActors;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), *TargetTag, TargetActors);

	AGlobalCharacter* Pawn = GetGlobalCharacter(OwnerComp);

	float SearchRange = Blackboard->GetValueAsFloat(TEXT("SearchRange"));

	AActor* ResultActor = nullptr;

	if (0 == TargetActors.Num())
	{
		return nullptr;
	}

	float NearDis = TNumericLimits<float>::Max();

	for (int i = 0; i < TargetActors.Num(); ++i)
	{
		FVector TargetDistance = Pawn->GetActorLocation() - TargetActors[i]->GetActorLocation();
		float TDisSize = TargetDistance.Size();

		if (SearchRange <= TDisSize)
		{
			continue;
		}
		else if (NearDis > TDisSize)
		{
			NearDis = TDisSize;
			ResultActor = TargetActors[i];
		}
	}

	return ResultActor;
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
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->DAndAButtonAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->RollorRunAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(0);

	return EBTNodeResult::InProgress;
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
