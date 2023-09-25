#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnPoint.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API ASpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	ASpawnPoint();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:


};
