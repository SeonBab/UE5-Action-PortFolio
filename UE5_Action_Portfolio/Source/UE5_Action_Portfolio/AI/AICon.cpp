#include "AI/AICon.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Global/GlobalAICharacter.h"
#include "Global/Enums.h"

AAICon::AAICon()
{
	// 2�� �迭�� ���� �����ϰ�, ������ ���� ���������� ģ������ �߸����� �����Ѵ�.
	Attitudes = {
		{ETeamAttitude::Friendly, ETeamAttitude::Neutral, ETeamAttitude::Neutral, ETeamAttitude::Neutral, ETeamAttitude::Neutral }, // Neutral
		{ETeamAttitude::Neutral, ETeamAttitude::Friendly, ETeamAttitude::Hostile, ETeamAttitude::Hostile, ETeamAttitude::Hostile }, // PlayerTeam
		{ETeamAttitude::Neutral, ETeamAttitude::Hostile, ETeamAttitude::Friendly, ETeamAttitude::Hostile, ETeamAttitude::Friendly }, // CloneTeam
		{ETeamAttitude::Neutral, ETeamAttitude::Hostile, ETeamAttitude::Hostile, ETeamAttitude::Friendly, ETeamAttitude::Friendly }, // MonsterTeam
		{ETeamAttitude::Neutral, ETeamAttitude::Hostile, ETeamAttitude::Friendly, ETeamAttitude::Friendly, ETeamAttitude::Friendly }, // BossTeam
	};

	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());

	GetAISenseConfigSight()->DetectionByAffiliation.bDetectEnemies = true;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectNeutrals = false;
	GetAISenseConfigSight()->DetectionByAffiliation.bDetectFriendlies = false;
}

void AAICon::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (true == TargetLost)
	{
		LostTimer += _DeltaTime;
	}

	if (5.f <= LostTimer)
	{
		LostTimer = 0.f;
		TargetLost = false;
		PerceivedActor = nullptr;
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
		GetBlackboardComponent()->SetValueAsBool(TEXT("IsReturn"), true);
	}
}

UBehaviorTreeComponent* AAICon::GetBehaviorTreeComponent()
{
	return BehaviorTreeComponent;
}

UBlackboardComponent* AAICon::GetBlackboardComponent()
{
	return BlackboardComponent;
}

UAIPerceptionComponent* AAICon::GetAIPerceptionComponent()
{
	return AIPerceptionComponent;
}

UAISenseConfig_Sight* AAICon::GetAISenseConfigSight()
{
	return AISenseConfigSight;
}

void AAICon::OnPossess(APawn* _InPawn)
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

	UBehaviorTreeComponent* CurBehaviorTreeComponent = GetBehaviorTreeComponent();

	if (false == IsValid(CurBehaviorTreeComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	AICharacter->AIInit();

	CurBehaviorTreeComponent->StartTree(*BehaviorTree);
}

void AAICon::BeginPlay()
{
	Super::BeginPlay();

	GetAIPerceptionComponent()->OnTargetPerceptionUpdated.AddDynamic(this, &AAICon::OnTargetPerceptionUpdated);
}

ETeamAttitude::Type AAICon::GetTeamAttitudeTowards(const AActor& Other) const
{
	APawn const* OtherPawn = Cast<APawn>(&Other);

	if (false == IsValid(OtherPawn))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return ETeamAttitude::Neutral;
	}

	AController* OtherPawnCon = OtherPawn->GetController();

	if (false == IsValid(OtherPawnCon))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return ETeamAttitude::Neutral;
	}

	const IGenericTeamAgentInterface* const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawnCon);

	if (nullptr == TeamAgent)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == TeamAgent"), __FUNCTION__, __LINE__);
		return ETeamAttitude::Neutral;
	}

	FGenericTeamId CurTeamID = this->GetGenericTeamId();
	FGenericTeamId OtehrTeamID = TeamAgent->GetGenericTeamId();

	bool CurTeamIDValid = Attitudes.IsValidIndex(CurTeamID.GetId());
	bool OtehrTeamIDValid = Attitudes.IsValidIndex(OtehrTeamID.GetId());

	if (true == CurTeamIDValid && true == OtehrTeamIDValid)
	{
		return Attitudes[CurTeamID.GetId()][OtehrTeamID.GetId()];
	}

	return ETeamAttitude::Neutral;
}

void AAICon::OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus _Stimulus)
{
	switch (_Stimulus.Type)
	{
	case _Stimulus.SensingSucceeded: // Ÿ�� �ν� ����
		// �ν��� Ÿ���� ���̶��
		if (ETeamAttitude::Hostile == GetTeamAttitudeTowards(*_Actor))
		{
			// �����忡 ����� Ÿ�� Ȯ��
			UObject* TargetObject = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));

			if (false == IsValid(TargetObject))
			{
				LostTimer = 0.f;
				TargetLost = false;
				PerceivedActor = nullptr;
			}

			// �ν� �ߴ� ���� ���ٸ�
			if (nullptr == PerceivedActor)
			{
				LostTimer = 0.f;
				TargetLost = false;
				PerceivedActor = _Actor;

			}
			// �ν� �ߴ� ���� �ִٸ�
			// ex) Ÿ���� ���� ���� ��
			// ex) �ν� ������ ���� Ÿ���� �ٽ� ���� ��
			// ex) �ν� ������ ������ Ÿ���� �ٽ� ���� ��
			else if (nullptr != PerceivedActor)
			{
				// �ν� �ߴ� ���� �������� Ÿ�ٰ� ����
				if (_Actor == TargetObject)
				{
					// ex) �ν� ������ ������ Ÿ���� �ٽ� ���� ��
					if (true == TargetLost)
					{
						LostTimer = 0.f;
						TargetLost = false;
					}
					// ex) �ν� ������ ���� Ÿ���� �ٽ� ���� ��
					else if (false == TargetLost)
					{
						TargetLost = true;
						return;
					}
				}
				// �ν� �ߴ� ���� �ٸ��ٸ�
				// ex) �ٸ� Ÿ���� ���� �����ų� ������
				else
				{
					APawn* CurPawn = GetPawn();

					if (false == IsValid(CurPawn))
					{
						UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
						return;
					}

					FVector CurLocation = CurPawn->GetActorLocation();
					FMath::Abs(CurLocation.X);
					FMath::Abs(CurLocation.Y);
					FMath::Abs(CurLocation.Z);

					FVector PerActorLocation = PerceivedActor->GetActorLocation();
					FMath::Abs(PerActorLocation.X);
					FMath::Abs(PerActorLocation.Y);
					FMath::Abs(PerActorLocation.Z);

					FVector _ActorLocation = _Actor->GetActorLocation();
					FMath::Abs(_ActorLocation.X);
					FMath::Abs(_ActorLocation.Y);
					FMath::Abs(_ActorLocation.Z);

					FVector PerceivedActorDis = PerActorLocation - CurLocation;
					FVector _ActorDis = _ActorLocation - CurLocation;

					// �ν� �ߴ� ������ ���� �ν� �� ���� �����ٸ� Ÿ�� ����
					if (PerceivedActorDis.Size() > _ActorDis.Size())
					{
						LostTimer = 0.f;
						TargetLost = false;
						PerceivedActor = _Actor;
					}
				}
			}

			GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PerceivedActor);
		}
		break;
	case _Stimulus.SensingFailed: // Ÿ�� �ν� ����
		PerceivedActor = nullptr;
		LostTimer = 0.f;
		TargetLost = false;
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
		break;
	default:
		return;
		break;
	}
}