#include "AI/BTTask/BTTask_Monster_Attack.h"

EBTNodeResult::Type UBTTask_Monster_Attack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalAICharacter* Character = GetGlobalAICharacter(OwnerComp);

    if (false == IsValid(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(AIAnimState::Attack);

    return EBTNodeResult::InProgress;
}

void UBTTask_Monster_Attack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AGlobalAICharacter* Character = GetGlobalAICharacter(OwnerComp);

    if (false == IsValid(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    UAnimMontage* Montage = Character->GetAnimMontage(Character->GetAnimState());

    if (false == IsValid(Montage))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return;
    }

    float Time = Montage->CalculateSequenceLength();

    if (Time <= GetStateTime(OwnerComp))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
