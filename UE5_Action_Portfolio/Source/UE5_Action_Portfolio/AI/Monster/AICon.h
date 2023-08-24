#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "AICon.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAICon : public AAIController
{
	GENERATED_BODY()
public:
	AAICon();
	virtual void Tick(float _DeltaTime) override;

protected:
	void OnPossess(APawn* _InPawn) override;

	virtual void BeginPlay() override;
	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;

	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* _Actor, FAIStimulus _Stimulus);

private:
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBehaviorTreeComponent* BehaviorTreeComponent = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UBlackboardComponent* BlackboardComponent = nullptr;
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UAIPerceptionComponent> AIPerceptionComponent = nullptr;
	UPROPERTY()
	TObjectPtr<class UAISenseConfig_Sight> AISenseConfigSight = nullptr;
	//TObjectPtr<class UAISenseConfig_Hearing> AISenseConfigHearing = nullptr;

	UPROPERTY()
	AActor* PerceivedActor = nullptr;
	bool TargetLost = false;
	float LostTimer = 0.f;
};
