#include "AI/Boss/AI_BossCon.h"
#include "Global/GlobalAICharacter.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_BossCon::AAI_BossCon()
{
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectEnemies = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectNeutrals = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectFriendlies = false;
	GetAISenseConfigSight()->SightRadius = 3000.f;
	GetAISenseConfigSight()->LoseSightRadius = 3500.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 360.f;
	GetAISenseConfigSight()->SetMaxAge(5.f);

	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

void AAI_BossCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAI_BossCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	if (nullptr == GetBehaviorTreeComponent() && false == GetBehaviorTreeComponent()->IsValidLowLevel())
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> if (nullptr == BehaviorTreeComponent && false == BehaviorTreeComponent->IsValidLowLevel())"), __FUNCTION__, __LINE__);
		return;
	}
	
	AGlobalAICharacter* AIPawn = Cast<AGlobalAICharacter>(_InPawn);
	
	UBehaviorTree* BehaviorTree = AIPawn->GetBehaviorTree();
	
	if (nullptr == BehaviorTree || false == BehaviorTree->IsValidLowLevel())
	{
		return;
	}
	
	GetBlackboardComponent()->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	GetBlackboardComponent()->SetValueAsObject(TEXT("SelfActor"), _InPawn);
	
	GetBehaviorTreeComponent()->StartTree(*BehaviorTree);
}

void AAI_BossCon::BeginPlay()
{
	Super::BeginPlay();
}
