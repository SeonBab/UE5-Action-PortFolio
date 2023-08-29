#include "AI/Boss/Tornado.h"

ATornado::ATornado()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 0.f;
	DeathTime = 0.f;
	Damage = 0.f;
}

void ATornado::TornadoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

