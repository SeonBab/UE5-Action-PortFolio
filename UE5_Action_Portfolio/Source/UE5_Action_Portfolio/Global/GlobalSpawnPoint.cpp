#include "Global/GlobalSpawnPoint.h"
#include "Global/GlobalGameInstance.h"
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
	SpawnCheck = false;

	CurSpawnActor = nullptr;
	SpawnCount = 0;
}

void AGlobalSpawnPoint::BeginPlay()
{
	Super::BeginPlay();
}

void AGlobalSpawnPoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == SpawnCheck)
	{
		if (true == IsValid(CurSpawnActor))
		{
			// 스폰 돼있는 액터가 존제한다면 리턴
			return;
		}
	}

	if (SpawnTime <= SpawnElapsedTime && MaximumSpawn > SpawnCount)
	{
		FActorSpawnParameters params;
		params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

		FVector CurLocation = GetActorLocation();

		float RandomX = UGlobalGameInstance::MainRandom.FRandRange(-RandomSpawnSizeX, RandomSpawnSizeX);
		float RandomY = UGlobalGameInstance::MainRandom.FRandRange(-RandomSpawnSizeY, RandomSpawnSizeY);

		CurLocation.X += RandomX;
		CurLocation.Y += RandomY;

		CurSpawnActor = GetWorld()->SpawnActor<AActor>(SpawnActorClass, CurLocation, GetActorRotation(), params);

		++SpawnCount;
		SpawnElapsedTime = 0.f;
	}
	else
	{
		SpawnElapsedTime += DeltaTime;
	}
}

