#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICon.generated.h"

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
	class UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = nullptr;
	TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight = nullptr;
	//TObjectPtr<class UAISenseConfig_Hearing> AISenseConfigHearing = nullptr;

};
