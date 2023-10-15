#include "AI/Boss/BTTask_Boss_Teleport_Hide.h"

EBTNodeResult::Type UBTTask_Boss_Teleport_Hide::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    AGlobalAICharacter* Character = GetGlobalAICharacter(OwnerComp);

    if (false == IsValid(Character))
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    Character->SetAnimState(BossAnimState::Hide);

	return EBTNodeResult::InProgress;
}

void UBTTask_Boss_Teleport_Hide::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
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
        UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

        if (false == IsValid(Blackboard))
        {
            UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
            return;
        }

        FVector TeleportPos = Blackboard->GetValueAsVector(TEXT("TeleportPos"));

        Character->SetActorLocation(TeleportPos);

        FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
    }
}
