#include "Global/GlobalCharacter.h"
#include "Components/AudioComponent.h"

AGlobalCharacter::AGlobalCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	ActorTypeTag = TEXT("");
	AttackTypeTag = TEXT("");
	Parrybool = false;
	IsInvincibility = false;
	HP = 0.f;
	MaxHP = 0.f;

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComponent"));
	AudioComponent->SetupAttachment(GetMesh());
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

void AGlobalCharacter::SetParrybool(bool _Value)
{
	Parrybool = _Value;
}

bool AGlobalCharacter::GetParrybool()
{
	return Parrybool;
}

bool AGlobalCharacter::GetIsInvincibility()
{
	return IsInvincibility;
}

void AGlobalCharacter::SetIsInvincibility(bool _Value)
{
	IsInvincibility = _Value;
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

UAudioComponent* AGlobalCharacter::GetAudioComponent()
{
	return AudioComponent;
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

