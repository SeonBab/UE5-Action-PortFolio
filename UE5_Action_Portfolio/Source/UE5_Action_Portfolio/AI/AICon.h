#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GlobalAIController.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAICon : public AAIController
{
	GENERATED_BODY()
public:
	AAICon();

protected:
	void OnPossess(APawn* _InPawn) override;

private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent;
};
