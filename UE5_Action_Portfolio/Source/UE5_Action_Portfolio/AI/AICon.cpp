#include "AI/AICon.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

#include "Global/GlobalAICharacter.h"

AAICon::AAICon()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigSight->SightRadius = 1000.f;
	AISenseConfigSight->LoseSightRadius = 2000.f;

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	
	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

void AAICon::OnPossess(APawn* _InPawn)
{
	Super::OnPossess(_InPawn);

	if (nullptr == BehaviorTreeComponent && false == BehaviorTreeComponent->IsValidLowLevel())
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

	BlackboardComponent->InitializeBlackboard(*BehaviorTree->BlackboardAsset);
	BlackboardComponent->SetValueAsObject(TEXT("SelfActor"), _InPawn);

	BehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AAICon::BeginPlay()
{
	Super::BeginPlay();

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAICon::OnTargetPerceptionUpdated_Delegate);
}

ETeamAttitude::Type AAICon::GetTeamAttitudeTowards(const AActor& Other) const
{
	APawn const* OtherPawn = Cast<APawn>(&Other);

	if (nullptr != OtherPawn)
	{
		IGenericTeamAgentInterface* const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());

		if (nullptr != TeamAgent)
		{
			FGenericTeamId OtehrTeamID = TeamAgent->GetGenericTeamId();

			if (1 == OtehrTeamID)
			{
				return ETeamAttitude::Friendly;
			}
			else
			{
				return ETeamAttitude::Hostile;
			}

		}
	}
	return ETeamAttitude::Neutral;
}

void AAICon::OnTargetPerceptionUpdated_Delegate(AActor* _Actor, FAIStimulus _Stimulus)
{
	int a = 0;
	switch (_Stimulus.Type)
	{
	case 0: // 타깃 인식 성공 SensingSucceeded
		a = 1;
		break;
	case 1: // 타깃 인식 실패 SensingFailed
		a = 2;
		break;
	default:
		return;
		break;
	}


}
