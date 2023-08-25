#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BTTask_AIBase.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	class ACharacter* GetCurCharacter(UBehaviorTreeComponent& OwnerComp);

	TArray<FVector> PathFind(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor);
	TArray<FVector> PathFind(UBehaviorTreeComponent& _OwnerComp, FVector _Pos);
	float GetStateTime(UBehaviorTreeComponent& OwnerComp);
	void ResetStateTime(UBehaviorTreeComponent& OwnerComp);
	UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp);

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	virtual void OnGameplayTaskActivated(class UGameplayTask& _Task) override;
	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
