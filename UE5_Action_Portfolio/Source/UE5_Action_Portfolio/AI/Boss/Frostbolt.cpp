#include "AI/Boss/Frostbolt.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AFrostbolt::AFrostbolt()
{
	Speed = 2000.f;
	DeathTime = 3.f;
	Damage = 10.f;
	IsSpawn = false;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->SetUpdatedComponent(SphereComponent);
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 2000.f;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(SphereComponent);

	GetNiagaraComponent()->SetupAttachment(SphereComponent);
}

USphereComponent* AFrostbolt::GetSphereComponent()
{
	return SphereComponent;
}

void AFrostbolt::MagicBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AFrostbolt::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFrostbolt::MagicBeginOverlap);
}

void AFrostbolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
