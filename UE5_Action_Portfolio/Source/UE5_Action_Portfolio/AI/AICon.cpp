#include "AI/AICon.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"

AAICon::AAICon()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));

	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());
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
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), PerceivedActor);
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
		return ETeamAttitude::Friendly;
	}

	const IGenericTeamAgentInterface* const TeamAgent = Cast<IGenericTeamAgentInterface>(OtherPawn->GetController());

	if (nullptr == TeamAgent)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == TeamAgent"), __FUNCTION__, __LINE__);
		return ETeamAttitude::Friendly;
	}

	FGenericTeamId OtehrTeamID = TeamAgent->GetGenericTeamId();

	if (1 == OtehrTeamID)
	{
		return ETeamAttitude::Friendly;
	}
	else
	{
		return ETeamAttitude::Hostile;
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
				UObject* TargetObject = GetBlackboardComponent()->GetValueAsObject(TEXT("TargetActor"));
				AActor* TargetActor = Cast<AActor>(TargetObject);

				if (false == IsValid(TargetActor))
				{
					UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
					return;
				}

				// �ν� �ߴ� ���� Ÿ�ٰ� ����
				if (_Actor == TargetActor)
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
					}
				}
				// �ν� �ߴ� ���� �ٸ��ٸ�
				// ex) �ٸ� Ÿ���� ���� �����ų� ������
				else
				{
					APawn* CurPawn = GetPawn();

					if (IsValid(CurPawn))
					{
						UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
						return;
					}

					FVector CurLocation = CurPawn->GetActorLocation();

					FVector PerceivedActorDis = PerceivedActor->GetActorLocation() - CurLocation;
					FVector TargetdActorDis = _Actor->GetActorLocation() - CurLocation;

					// �ν� �ߴ� ������ ���� �ν� �� ���� �����ٸ� Ÿ�� ����
					if (PerceivedActorDis.Size() > TargetdActorDis.Size())
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
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
		break;
	default:
		return;
		break;
	}
}
