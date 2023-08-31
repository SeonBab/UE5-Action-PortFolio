#include "AI/Boss/Magic.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"

AMagic::AMagic()
{
	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));

	Speed = 0.f;
	DeathTime = 5.f;
	Damage = 10.f;
}

USphereComponent* AMagic::GetSphereComponent()
{
	return SphereComponent;
}

void AMagic::MagicBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FPointDamageEvent DamageEvent;

	AController* Controller = GetCurController();

	if (nullptr == Controller)
	{
		return;
	}

	OtherActor->TakeDamage(Damage, DamageEvent, Controller, this);

	Destroy();
}

void AMagic::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AMagic::MagicBeginOverlap);
}

void AMagic::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
