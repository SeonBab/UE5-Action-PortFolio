#include "AI/Boss/BTTask_Attack_Frostbolt.h"

EBTNodeResult::Type UBTTask_Attack_Frostbolt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalAICharacter* Character = GetGlobalAICharacter(OwnerComp);

    if (false == IsValid(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Attack_FrostboltShot);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack_Frostbolt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
