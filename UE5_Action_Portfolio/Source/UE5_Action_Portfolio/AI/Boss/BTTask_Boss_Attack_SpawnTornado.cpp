#include "AI/Boss/BTTask_Boss_Attack_SpawnTornado.h"

EBTNodeResult::Type UBTTask_Boss_Attack_SpawnTornado::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character || false == Character->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Attack_SpawnTornado);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Attack_SpawnTornado::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character || false == Character->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return;
    }

    UAnimMontage* Montage = Character->GetAnimMontage(Character->GetAnimState());

    if (nullptr == Montage || false == Montage->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return;
    }

    float Time = Montage->CalculateSequenceLength();

    if (Time <= GetStateTime(OwnerComp))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
