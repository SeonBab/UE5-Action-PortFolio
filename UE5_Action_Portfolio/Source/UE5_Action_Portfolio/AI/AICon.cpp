#include "AI/AICon.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "Perception/AIPerceptionComponent.h"
#include "Global/GlobalAICharacter.h"

AAICon::AAICon()
{
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	AIPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerceptionComponent"));
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
