#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Monster/AICon.h"
#include "Global/Enums.h"
#include "Global/GlobalCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Weapon/WeaponAction.h"
#include "NavigationSystem.h"
#include "BTTask_AIBase.generated.h"

/**
 * 
 */
UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_AIBase : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:
	UBTTask_AIBase();
	class AGlobalCharacter* GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp);
	float GetStateTime(UBehaviorTreeComponent& OwnerComp);
	void ResetStateTime(UBehaviorTreeComponent& OwnerComp);
	class UBlackboardComponent* GetBlackboardComponent(UBehaviorTreeComponent& OwnerComp);
	TArray<FVector> PathFind(UBehaviorTreeComponent& _OwnerComp, AActor* _Actor);
	TArray<FVector> PathFind(UBehaviorTreeComponent& _OwnerComp, FVector _Pos);

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory);
	void OnGameplayTaskActivated(class UGameplayTask& _Task) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
