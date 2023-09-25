#pragma once

#include "CoreMinimal.h"
#include "AI/AICon.h"
#include "AIMonsterCon.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAIMonsterCon : public AAICon
{
	GENERATED_BODY()

public:
	AAIMonsterCon();


protected:
	void Tick(float _DeltaTime) override;
	void OnPossess(APawn* _InPawn) override;
	void BeginPlay() override;
	
};
