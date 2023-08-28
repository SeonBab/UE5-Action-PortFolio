#pragma once

#include "CoreMinimal.h"
#include "AI/AICon.h"
#include "AI_BossCon.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAI_BossCon : public AAICon
{
	GENERATED_BODY()

public:
	AAI_BossCon();
	void Tick(float _DeltaTime) override;

protected:
	void OnPossess(APawn* _InPawn) override;
	void BeginPlay() override;
};
