#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTask_MonsterBase.h"
#include "BTTask_Clone_Patrol.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Clone_Patrol : public UBTTask_MonsterBase
{
	GENERATED_BODY()
	

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
