#include "AI/Monster/AIPlayerCloneCon.h"
#include "AI/Monster/CloneMonster.h"
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

	ACloneMonster* AIPawn = Cast<ACloneMonster>(_InPawn);
	
	UBehaviorTree* BehaviorTree = AIPawn->GetBehaviorTree();

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

void AAIPlayerCloneCon::BeginPlay()
{
	Super::BeginPlay();
}
