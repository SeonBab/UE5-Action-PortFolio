#include "AI/Boss/AI_BossCon.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Enums.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAI_BossCon::AAI_BossCon()
{
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectEnemies = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectNeutrals = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectFriendlies = false;
	GetAISenseConfigSight()->SightRadius = 4000.f;
	GetAISenseConfigSight()->LoseSightRadius = 4500.f;
	GetAISenseConfigSight()->PeripheralVisionAngleDegrees = 360.f;
	GetAISenseConfigSight()->SetMaxAge(5.f);

	int Team = static_cast<int>(TeamType::BossTeam);
	AAIController::SetGenericTeamId(FGenericTeamId(Team));
}

void AAI_BossCon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
}

void AAI_BossCon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	AGlobalAICharacter* AICharacter = Cast<AGlobalAICharacter>(_InPawn);

	if (false == IsValid(AICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}
	
	UBehaviorTree* BehaviorTree = AICharacter->GetBehaviorTree();
	
	if (false == IsValid(BehaviorTree))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UBlackboardComponent* CurBlackboardComponent = GetBlackboardComponent();

	if (false == IsValid(CurBlackboardComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}
	
	CurBlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	CurBlackboardComponent->SetValueAsObject(TEXT("SelfActor"), _InPawn);
	
	UBehaviorTreeComponent* CurBehaviorTreeComponent = GetBehaviorTreeComponent();

	if (false == IsValid(CurBehaviorTreeComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurBehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AAI_BossCon::BeginPlay()
{
	Super::BeginPlay();
}
