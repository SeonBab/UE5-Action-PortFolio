#include "AI/Monster/AIPlayerCloneCon.h"
#include "AI/Monster/AIPlayerCloneCharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAIPlayerCloneCon::AAIPlayerCloneCon()
{	
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectEnemies = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectNeutrals = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectFriendlies = false;
	GetAISenseConfigSight()->SightRadius = 2000.f;
	GetAISenseConfigSight()->LoseSightRadius = 3500.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 90.f;
	GetAISenseConfigSight()->SetMaxAge(5.f);

	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

void AAIPlayerCloneCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAIPlayerCloneCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	if (nullptr == GetBehaviorTreeComponent() && false == GetBehaviorTreeComponent()->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == BehaviorTreeComponent && false == BehaviorTreeComponent->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return;
	}

	AAIPlayerCloneCharacter* AIPawn = Cast<AAIPlayerCloneCharacter>(_InPawn);
	
	UBehaviorTree* BehaviorTree = AIPawn->GetBehaviorTree();

	if (nullptr == BehaviorTree || false == BehaviorTree->IsValidLowLevel())
	{
		return;
	}

	GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), _InPawn);

	GetBehaviorTreeComponent()->StartTree(*BehaviorTree);
}

void AAIPlayerCloneCon::BeginPlay()
{
	Super::BeginPlay();
}
