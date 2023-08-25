#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AICon.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAICon : public AAIController
{
	GENERATED_BODY()

public:
	AAICon();

	UBehaviorTreeComponent* GetBehaviorTreeComponent();
	UBlackboardComponent* GetBlackboardComponent();

private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent = nullptr;
};
