#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTask_Clone_Base.h"
#include "BTTask_Clone_WeaponMove.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Clone_WeaponMove : public UBTTask_Clone_Base
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	int MoveTime = 0;
};
