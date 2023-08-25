#include "AI/Boss/Lich.h"

ALich::ALich()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorTypeTag = TEXT("");
	AttackTypeTag = TEXT("");
}

void ALich::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALich::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

