// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTask_AIBase.h"
#include "BTTask_AIBase.h"

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

void UBTTask_AIBase::OnGameplayTaskActivated(UGameplayTask& _Task)
{
	
}
