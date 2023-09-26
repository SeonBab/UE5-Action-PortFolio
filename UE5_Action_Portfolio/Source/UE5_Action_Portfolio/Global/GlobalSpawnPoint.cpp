#include "Global/GlobalSpawnPoint.h"
#include "GameFramework/Pawn.h"

AGlobalSpawnPoint::AGlobalSpawnPoint()
{
	PrimaryActorTick.bCanEverTick = true;

	SpawnActorClass = nullptr;
	SpawnTime = 0.f;
	SpawnElapsedTime = 0.f;
	MaximumSpawn = 0;
	RandomSpawnSizeX = 0.f;
	RandomSpawnSizeY = 0.f;

	SpawnCount = 0;
}

void AGlobalSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AGlobalSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (SpawnTime <= SpawnElapsedTime && MaximumSpawn > SpawnCount)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
		AActor* CurSpawnActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, GetActorLocation(), GetActorRotation(), params);
		APawn* CurPawn = Cast<APawn>(CurSpawnActor);

		if (true == IsValid(CurPawn))
		{
			CurPawn->SpawnDefaultController();
		}

		++SpawnCount;
		SpawnElapsedTime = 0.f;
	}
	else
	{
		SpawnElapsedTime += DeltaTime;
	}
}

