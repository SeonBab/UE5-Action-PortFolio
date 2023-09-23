#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/BehaviorTree.h"
#include "AICon.generated.h"

class UBehaviorTreeComponent;
class UBlackboardComponent;
class UAISenseConfig_Sight;

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAICon : public AAIController
{
	GENERATED_BODY()

public:
	AAICon();

	UBehaviorTreeComponent* GetBehaviorTreeComponent();
	UBlackboardComponent* GetBlackboardComponent();
	UAIPerceptionComponent* GetAIPerceptionComponent();
	UAISenseConfig_Sight* GetAISenseConfigSight();

protected:
	void Tick(float _DeltaTime) override;
	virtual void OnPossess(APawn* _InPawn) override;
	virtual void BeginPlay() override;
	ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus _Stimulus);

private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBlackboardComponent* BlackboardComponent = nullptr;

	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = nullptr;
	UPROPERTY()
	TObjectPtr<UAISenseConfig_Sight> AISenseConfigSight = nullptr;

	UPROPERTY()
	AActor* PerceivedActor = nullptr;
	bool TargetLost = false;
	float LostTimer = 0.f;

	// �� �ν� �迭
	TArray<TArray<ETeamAttitude::Type>> Attitudes;
};
