#include "AI/Monster/BTTask_Patrol.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

EBTNodeResult::Type UBTTask_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

    int PatrolCount = Blackboard->GetValueAsInt(TEXT("PatrolCount"));
    ++PatrolCount;

    if (5 < PatrolCount)
    {
        Blackboard->SetValueAsBool(TEXT("IsReturn"), true);
        return EBTNodeResult::Failed;
    }

    Blackboard->SetValueAsInt(TEXT("PatrolCount"), PatrolCount);

    UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetNavigationSystem(GetWorld());

    if (nullptr == NavSystem)
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = GetGlobalCharacter(OwnerComp)->GetActorLocation();


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