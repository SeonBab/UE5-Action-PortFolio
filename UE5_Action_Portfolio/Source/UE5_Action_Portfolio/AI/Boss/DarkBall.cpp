#include "AI/Boss/DarkBall.h"

ADarkBall::ADarkBall()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName("NoCollision", true);

	Speed = 0.f;
	DeathTime = 5.f;
}

void ADarkBall::SetDeathCheck(bool _Value)
{
	DeathCheck = _Value;
}

void ADarkBall::SetSpeed(float _Value)
{
	Speed = _Value;
}

void ADarkBall::BeginPlay()
{
	Super::BeginPlay();

	OnDestroyed.AddDynamic(this, &ADarkBall::DestroyProjectile);
}

void ADarkBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == DeathCheck)
	{
		DeathTime -= DeltaTime;
	}

	if (DeathTime < 0.0f)
	{
		Destroy();
		return;
	}

	AddActorWorldOffset(GetActorForwardVector() * DeltaTime * Speed);
}

void ADarkBall::DestroyProjectile(AActor* _Destroy)
{
	if (nullptr == DeathCreateObject)
	{
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor<AActor>(DeathCreateObject);

	Actor->SetActorLocation(GetActorLocation());
	Actor->SetActorRotation(GetActorRotation());
}

