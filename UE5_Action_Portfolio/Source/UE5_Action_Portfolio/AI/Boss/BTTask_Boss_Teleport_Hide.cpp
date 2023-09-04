#include "AI/Boss/BTTask_Boss_Teleport_Hide.h"

EBTNodeResult::Type UBTTask_Boss_Teleport_Hide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalCharacter* Character = GetGlobalCharacter(OwnerComp);

    if (nullptr == Character)
    {
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Hide);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Teleport_Hide::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
        UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

        if (nullptr == Blackboard)
        {
            return;
        }

        FVector TeleportPos = Blackboard->GetValueAsVector(TEXT("TeleportPos"));

        Character->SetActorLocation(TeleportPos);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
