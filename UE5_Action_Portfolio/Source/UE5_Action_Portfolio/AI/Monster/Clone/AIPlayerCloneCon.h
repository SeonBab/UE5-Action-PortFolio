#pragma once

#include "CoreMinimal.h"
#include "AI/AICon.h"
#include "AIPlayerCloneCon.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AAIPlayerCloneCon : public AAICon
{
	GENERATED_BODY()
public:
	AAIPlayerCloneCon();
	void Tick(float _DeltaTime) override;

protected:
	void OnPossess(APawn* _InPawn) override;
	void BeginPlay() override;
};
