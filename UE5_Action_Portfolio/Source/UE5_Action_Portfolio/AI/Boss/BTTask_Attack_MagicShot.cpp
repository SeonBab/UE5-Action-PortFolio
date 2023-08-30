#include "AI/Boss/BTTask_Attack_MagicShot.h"

EBTNodeResult::Type UBTTask_Attack_MagicShot::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character)
    {
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Attack_MagicShot);

	return EBTNodeResult::InProgress;
}

void UBTTask_Attack_MagicShot::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
