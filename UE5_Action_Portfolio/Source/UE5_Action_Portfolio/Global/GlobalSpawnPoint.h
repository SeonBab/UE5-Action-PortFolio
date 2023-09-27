#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GlobalSpawnPoint.generated.h"

UCLASS()
class UE5_ACTION_PORTFOLIO_API AGlobalSpawnPoint : public AActor
{
	GENERATED_BODY()
	
public:	
	AGlobalSpawnPoint();

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> SpawnActorClass;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	float SpawnTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	float SpawnElapsedTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	int MaximumSpawn;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	float RandomSpawnSizeX;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	float RandomSpawnSizeY;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Meta = (AllowPrivateAccess = "true"))
	bool SpawnCheck;

	AActor* CurSpawnActor;
	int SpawnCount;
};
