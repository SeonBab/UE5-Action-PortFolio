#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AI/Monster/BTTask_MonsterBase.h"
#include "AI/Monster/Clone/CloneMonster.h"
#include "Global/GlobalGameInstance.h"
#include "Weapon/WeaponComponent.h"
#include "BTTask_Clone_Base.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API UBTTask_Clone_Base : public UBTTask_MonsterBase
{
	GENERATED_BODY()
	
public:
	UBTTask_Clone_Base();

	ACloneMonster* GetCloneMonster(UBehaviorTreeComponent& OwnerComp);

protected:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void OnGameplayTaskActivated(class UGameplayTask&) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
