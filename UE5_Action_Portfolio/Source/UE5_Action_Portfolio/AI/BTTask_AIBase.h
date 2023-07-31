#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include <Global/GlobalCharacter.h>
#include "BTTask_AIBase.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_AIBase();

	
};
