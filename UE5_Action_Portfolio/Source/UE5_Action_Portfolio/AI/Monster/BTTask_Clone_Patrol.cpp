#include "AI/Monster/BTTask_Clone_Patrol.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"

EBTNodeResult::Type UBTTask_Clone_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
    UBlackboardComponent* Blackboard = GetBlackboardComponent(OwnerComp);

    if (false == IsValid(Blackboard))
    {

        return EBTNodeResult::Failed;
    }

    if (true == Blackboard->GetValueAsBool(TEXT("IsReturn")))
    {
        return EBTNodeResult::Failed;
    }

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

    ACloneMonster* CloneMonster = GetCloneMonster(OwnerComp);

    if (false == IsValid(CloneMonster))
    {
        return EBTNodeResult::Failed;
    }

    FVector Origin = CloneMonster->GetActorLocation();


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

void UBTTask_Clone_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{

}