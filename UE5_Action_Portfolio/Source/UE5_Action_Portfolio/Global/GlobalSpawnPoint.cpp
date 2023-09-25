#include "Global/SpawnPoint.h"

ASpawnPoint::ASpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

}

void ASpawnPoint::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

