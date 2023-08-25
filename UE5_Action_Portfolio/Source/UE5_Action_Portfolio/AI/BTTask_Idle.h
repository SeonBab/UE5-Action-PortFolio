#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTask_MonsterBase.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_Idle.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Idle : public UBTTask_MonsterBase
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds);
};
