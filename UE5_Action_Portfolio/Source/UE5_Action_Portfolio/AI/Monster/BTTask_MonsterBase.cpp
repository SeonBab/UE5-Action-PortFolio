#include "AI/Monster/BTTask_MonsterBase.h"
#include "Kismet/GamePlayStatics.h"
#include "AI/Monster/AIPlayerCloneCon.h"

UBTTask_MonsterBase::UBTTask_MonsterBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

AGlobalCharacter* UBTTask_MonsterBase::GetGlobalCharacter(UBehaviorTreeComponent& OwnerComp)
{
	ACharacter* CurCharacter = GetCurCharacter(OwnerComp);

	if (nullptr == CurCharacter || false == CurCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == CurCharacter || false == CurCharacter->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	AGlobalCharacter* Character = Cast<AGlobalCharacter>(CurCharacter);

	if (nullptr == Character || false == Character->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == Character || false == Character->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	return Character;
}

EBTNodeResult::Type UBTTask_MonsterBase::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ResetStateTime(OwnerComp);

	GetGlobalCharacter(OwnerComp)->CurWeaponAction->WAndSButtonAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->DAndAButtonAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->RollorRunAction(0);
	GetGlobalCharacter(OwnerComp)->CurWeaponAction->AimorBlockAtion(0);

	return EBTNodeResult::InProgress;
}

void UBTTask_MonsterBase::OnGameplayTaskActivated(UGameplayTask& _Task)
{
	Super::OnGameplayTaskActivated(_Task);
}

void UBTTask_MonsterBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
