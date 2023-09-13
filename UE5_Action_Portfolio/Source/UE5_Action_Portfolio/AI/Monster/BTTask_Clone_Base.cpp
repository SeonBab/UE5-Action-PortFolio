#include "AI/Monster/BTTask_Clone_Base.h"
#include "Kismet/GamePlayStatics.h"
#include "AI/Monster/AIPlayerCloneCon.h"

UBTTask_Clone_Base::UBTTask_Clone_Base()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

ACloneMonster* UBTTask_Clone_Base::GetCloneMonster(UBehaviorTreeComponent& OwnerComp)
{
	AGlobalCharacter* GlobalCharacter = GetGlobalCharacter(OwnerComp);

	ACloneMonster* CloneCharacter = Cast<ACloneMonster>(GlobalCharacter);

	if (nullptr == CloneCharacter || false == CloneCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == CloneCharacter || false == CloneCharacter->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return CloneCharacter;
}

EBTNodeResult::Type UBTTask_Clone_Base::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	GetCloneMonster(OwnerComp)->GetWeaponComponent()->WAndSButtonAction(0);
	GetCloneMonster(OwnerComp)->GetWeaponComponent()->DAndAButtonAction(0);
	GetCloneMonster(OwnerComp)->GetWeaponComponent()->RollorRunAction(0);
	GetCloneMonster(OwnerComp)->GetWeaponComponent()->AimorBlockAtion(0);

	return EBTNodeResult::InProgress;
}

void UBTTask_Clone_Base::OnGameplayTaskActivated(UGameplayTask&)
{

}

void UBTTask_Clone_Base::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
