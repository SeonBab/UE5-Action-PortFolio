#include "AI/Monster/AIPlayerCloneCon.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AI/Monster/AIPlayerCloneCharacter.h"


AAIPlayerCloneCon::AAIPlayerCloneCon()
{
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
	AISenseConfigSight = CreateDefaultSubobject<UAISenseConfig_Sight>("SenseSight");
	
	AISenseConfigSight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectNeutrals = true;
	AISenseConfigSight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfigSight->SightRadius = 2000.f;
	AISenseConfigSight->LoseSightRadius = 3500.f;
	AISenseConfigSight->PeripheralVisionAngleDegrees = 90.f;
	AISenseConfigSight->SetMaxAge(5.f);

	AIPerceptionComponent->ConfigureSense(*AISenseConfigSight);
	AIPerceptionComponent->SetDominantSense(UAISenseConfig_Sight::StaticClass());
	AIPerceptionComponent->SetDominantSense(AISenseConfigSight->GetSenseImplementation());

	AAIController::SetGenericTeamId(FGenericTeamId(1));
}

void AAIPlayerCloneCon::Tick(float _DeltaTime)
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

	AIPerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AAIPlayerCloneCon::OnTargetPerceptionUpdated);
}

ETeamAttitude::Type AAIPlayerCloneCon::GetTeamAttitudeTowards(const AActor& Other) const
{
	APawn const* OtherPawn = Cast<APawn>(&Other);

	if (nullptr != OtherPawn)
	{	
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

void AAIPlayerCloneCon::OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus _Stimulus)
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

				if (nullptr == TargetObject)
				{
					return;
				}

				AActor* TargetActor = Cast<AActor>(TargetObject);

				if (nullptr == TargetActor)
				{
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
					if (nullptr == GetPawn() && false == GetPawn()->IsValidLowLevel())
					{
						return;
					}
					FVector CurLocation = GetPawn()->GetActorLocation();

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
