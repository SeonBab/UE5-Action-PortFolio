#include "AI/Boss/DarkBall.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"

ADarkBall::ADarkBall()
{
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName("NoCollision", true);
	SphereComponent->SetWorldScale3D({ 1.2f, 1.2f, 1.2f });

	GetNiagaraComponent()->SetupAttachment(SphereComponent);
	GetNiagaraComponent()->SetWorldScale3D({ 0.9f, 0.9f, 0.9f });

	Speed = 0.f;
	DeathTime = 5.f;
	Damage = 10.f;
}

USphereComponent* ADarkBall::GetSphereComponent()
{
	return SphereComponent;
}

void ADarkBall::DarkBallBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void ADarkBall::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &ADarkBall::DarkBallBeginOverlap);
	OnDestroyed.AddDynamic(this, &ADarkBall::DestroyProjectile);
}

void ADarkBall::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

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

