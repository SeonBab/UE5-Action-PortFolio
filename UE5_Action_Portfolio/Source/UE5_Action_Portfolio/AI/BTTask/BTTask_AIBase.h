#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"
#include "Global/Enums.h"
#include "BTTask_AIBase.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_AIBase();

	class AGlobalCharacter* GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp);

	TArray<FVector> PathFind(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor);
	TArray<FVector> PathFind(UBehaviorTreeComponent& _OwnerComp, FVector _Pos);
	UNavigationPath* PathFindNavPath(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor);
	UNavigationPath* PathFindNavPath(UBehaviorTreeComponent& _OwnerComp, FVector _Pos);

	float GetStateTime(UBehaviorTreeComponent& OwnerComp);
	void ResetStateTime(UBehaviorTreeComponent& OwnerComp);
	UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp);

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnGameplayTaskActivated(class UGameplayTask&) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
