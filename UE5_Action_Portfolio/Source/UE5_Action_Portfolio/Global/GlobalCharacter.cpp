#include "Global/GlobalCharacter.h"

AGlobalCharacter::AGlobalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	HP = 0.f;

}

void AGlobalCharacter::SetActorTypeTag(FName _Tag)
{
	ActorTypeTag = _Tag;
}

FName AGlobalCharacter::GetActorTypeTag()
{
	return ActorTypeTag;
}

void AGlobalCharacter::SetAttackTypeTag(FName _Tag)
{
	AttackTypeTag = _Tag;
}

FName AGlobalCharacter::GetAttackTypeTag()
{
	return AttackTypeTag;
}

void AGlobalCharacter::SetHP(float _HP)
{
	HP = _HP;
}

float AGlobalCharacter::GetHP()
{
	return HP;
}

void AGlobalCharacter::SetMaxHP(float _MaxHP)
{
	MaxHP = _MaxHP;
}

float AGlobalCharacter::GetMaxHP()
{
	return MaxHP;
}

void AGlobalCharacter::SetAnimState(int _AnimState)
{
	Animstate = _AnimState;
}

void AGlobalCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	Tags.Add(ActorTypeTag);
	Tags.Add(AttackTypeTag);
}

void AGlobalCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AGlobalCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

