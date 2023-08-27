#include "AI/Monster/BTTask_MonsterBase.h"
#include "Kismet/GamePlayStatics.h"
#include "AI/Monster/AIPlayerCloneCon.h"

UBTTask_MonsterBase::UBTTask_MonsterBase()
{
	bNotifyTick = true;
	bNotifyTaskFinished = true;
}

AWeaponCharacter* UBTTask_MonsterBase::GetWeaponCharacter(UBehaviorTreeComponent& OwnerComp)
{
	ACharacter* CurCharacter = GetGlobalCharacter(OwnerComp);

	if (nullptr == CurCharacter || false == CurCharacter->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == CurCharacter || false == CurCharacter->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return nullptr;
	}

	AWeaponCharacter* Character = Cast<AWeaponCharacter>(CurCharacter);

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

	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->WAndSButtonAction(0);
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->DAndAButtonAction(0);
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->RollorRunAction(0);
	GetWeaponCharacter(OwnerComp)->GetCurWeaponAction()->AimorBlockAtion(0);

	return EBTNodeResult::InProgress;
}

void UBTTask_MonsterBase::OnGameplayTaskActivated(UGameplayTask&)
{

}

void UBTTask_MonsterBase::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
