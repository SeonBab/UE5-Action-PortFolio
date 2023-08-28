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

ETeamAttitude::Type AAICon::GetTeamAttitudeTowards(const AActor& Other) const
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

void AAICon::OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus _Stimulus)
{
	switch (_Stimulus.Type)
	{
	case _Stimulus.SensingSucceeded: // 타겟 인식 성공
		// 인식한 타겟이 적이라면
		if (ETeamAttitude::Hostile == GetTeamAttitudeTowards(*_Actor))
		{
			// 인식 했던 적이 없다면
			if (nullptr == PerceivedActor)
			{
				LostTimer = 0.f;
				TargetLost = false;
				PerceivedActor = _Actor;

			}
			// 인식 했던 적이 있다면
			// ex) 타겟이 새로 들어올 때
			// ex) 인식 범위로 들어온 타겟이 다시 나갈 때
			// ex) 인식 범위를 나갔던 타겟이 다시 들어올 때
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

				// 인식 했던 적이 타겟과 같다
				if (_Actor == TargetActor)
				{
					// ex) 인식 범위를 나갔던 타겟이 다시 들어올 때
					if (true == TargetLost)
					{
						LostTimer = 0.f;
						TargetLost = false;
					}
					// ex) 인식 범위로 들어온 타겟이 다시 나갈 때
					else if (false == TargetLost)
					{
						TargetLost = true;
					}
				}
				// 인식 했던 적이 다르다면
				// ex) 다른 타겟이 새로 들어오거나 나갈때
				else
				{
					if (nullptr == GetPawn() && false == GetPawn()->IsValidLowLevel())
					{
						return;
					}
					FVector CurLocation = GetPawn()->GetActorLocation();

					FVector PerceivedActorDis = PerceivedActor->GetActorLocation() - CurLocation;
					FVector TargetdActorDis = _Actor->GetActorLocation() - CurLocation;

					// 인식 했던 적보다 새로 인식 된 적이 가깝다면 타겟 변경
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
	case _Stimulus.SensingFailed: // 타깃 인식 실패
		PerceivedActor = nullptr;
		GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
		break;
	default:
		return;
		break;
	}
}
