#include "AI/AICon.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Character/PlayerCon.h"
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
	AISenseConfigSight->SightRadius = 1100.f;
	AISenseConfigSight->LoseSightRadius = 1500.f;
	AISenseConfigSight->PeripheralVisionAngleDegrees = 90.f;
	AISenseConfigSight->SetMaxAge(5.f);

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());

	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

void AAICon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);
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

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAICon::OnTargetPerceptionUpdated);
}

ETeamAttitude::Type AAICon::GetTeamAttitudeTowards(const AActor& Other) const
{
	APawn const* OtherPawn = Cast<APawn>(&Other);

	if (nullptr != OtherPawn)
	{	
		// 왜 nullptr이 계속 리턴되는가????
		const IGenericTeamAgentInterface* const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());

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

void AAICon::OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus _Stimulus)
{
	switch (_Stimulus.Type)
	{
	case _Stimulus.SensingSucceeded: // 타깃 인식 성공
		// 인식은 잘 되는데 GetTeamAttitudeTowards 함수에 문제가 있다.
		if (ETeamAttitude::Hostile == GetTeamAttitudeTowards(*_Actor))
		{
			//PerceivedActor = _Actor;
			//BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), PerceivedActor);
		}

		if (nullptr != PerceivedActor)
		{
			PerceivedActor = nullptr;
		}
		else
		{
			PerceivedActor = _Actor;
		}
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), PerceivedActor);
		break;
	case _Stimulus.SensingFailed: // 타깃 인식 실패
		PerceivedActor = nullptr;
		BlackboardComponent->SetValueAsObject(TEXT("TargetActor"), nullptr);
		break;
	default:
		return;
		break;
	}
	
}
