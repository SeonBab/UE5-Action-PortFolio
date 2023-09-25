#pragma once

#include "CoreMinimal.h"
#include "AI/BTTask/BTTask_AIBase.h"
#include "BTTask_Boss_Teleport_Visible.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Boss_Teleport_Visible : public UBTTask_AIBase
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
