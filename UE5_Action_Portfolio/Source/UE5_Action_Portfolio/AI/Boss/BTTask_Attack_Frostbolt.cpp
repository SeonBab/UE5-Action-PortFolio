#include "AI/Boss/BTTask_Attack_Frostbolt.h"

EBTNodeResult::Type UBTTask_Attack_Frostbolt::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character)
    {
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Attack_FrostboltShot);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack_Frostbolt::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character)
    {
        return;
    }

    UAnimMontage* Montage = Character->GetAnimMontage(Character->GetAnimState());
    float Time = Montage->CalculateSequenceLength();

    if (Time <= GetStateTime(OwnerComp))
    {
        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
