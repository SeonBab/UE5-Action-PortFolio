#pragma once

#include "CoreMinimal.h"
#include "AI/Boss/BTTask_BossBase.h"
#include "BTTask_Boss_RandAttack.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Boss_RandAttack : public UBTTask_BossBase
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
