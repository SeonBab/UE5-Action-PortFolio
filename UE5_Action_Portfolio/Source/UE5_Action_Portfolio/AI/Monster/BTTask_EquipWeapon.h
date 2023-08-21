#pragma once

#include "CoreMinimal.h"
#include "AI/Monster/BTTask_AIBase.h"
#include "BTTask_EquipWeapon.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_EquipWeapon : public UBTTask_AIBase
{
	GENERATED_BODY()
	
protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
};
