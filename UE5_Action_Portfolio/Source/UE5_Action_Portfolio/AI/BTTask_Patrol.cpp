#include "AI/BTTask_Patrol.h"

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

    if (nullptr == NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = GetGlobalCharacter(OwnerComp)->GetActorLocation();


    UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);
    float Radius = Blackboard->GetValueAsFloat(TEXT("PatrolRange"));

    FNavLocation NextPatrol;

    NavSystem->GetRandomReachablePointInRadius(Origin, Radius, NextPatrol);

    if (false == NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsVector(TEXT("PatrolPos"), NextPatrol.Location);

    return EBTNodeResult::Succeeded;
}

void UBTTask_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}