// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "AI/AICon.h"
#include "Global/GlobalAICharacter.h"
#include "Weapon/WeaponAction.h"
#include "BTTask_AIBase.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_AIBase();

	class AGlobalCharacter* GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp);
protected:
	void OnGameplayTaskActivated(class UGameplayTask& _Task) override;
};
