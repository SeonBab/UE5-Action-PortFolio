#include "AI/Boss/BTTask_Boss_RandAttack.h"

EBTNodeResult::Type UBTTask_Boss_RandAttack::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    Super::ExecuteTask(OwnerComp, NodeMemory);

    UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

    if (nullptr == Blackboard || false == Blackboard->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    UObject* TargetObject = Blackboard->GetValueAsObject(TEXT("TargetActor"));
    
    if (nullptr == TargetObject || false == TargetObject->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }
    
    AActor* TargetActor = Cast<AActor>(TargetObject);

    if (nullptr == TargetActor || false == TargetActor->IsValidLowLevel())
    {
        UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr or IsValidLowLevel"), __FUNCTION__, __LINE__);
        return EBTNodeResult::Failed;
    }

    FVector TargetPos = TargetActor->GetActorLocation();
    FVector CurPos = GetGlobalCharacter(OwnerComp)->GetActorLocation();

    TargetPos.Z = 0.f;
    CurPos.Z = 0.f;

    FVector Dis = TargetPos - CurPos;

    int RandAttack = 0;

    float MeleeAttackRange = Blackboard->GetValueAsFloat(TEXT("MeleeAttackRange"));

    if (MeleeAttackRange >= Dis.Size())
    {
        RandAttack = UGlobalGameInstance::MainRandom.RandRange(1, 4);
    }
    else if (MeleeAttackRange < Dis.Size())
    {
        RandAttack = UGlobalGameInstance::MainRandom.RandRange(2, 4);
    }

    Blackboard->SetValueAsInt(TEXT("RandAttack"), RandAttack);

    return EBTNodeResult::Succeeded;
}

void UBTTask_Boss_RandAttack::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
    Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
}
