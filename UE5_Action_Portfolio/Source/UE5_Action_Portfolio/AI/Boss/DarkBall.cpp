#include "AI/Boss/DarkBall.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADarkBall::ADarkBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 20000.f;
	DeathTime = 3.f;
	Damage = 10.f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName("NoCollision", true);
	SphereComponent->SetWorldScale3D({ 1.2f, 1.2f, 1.2f });

	GetNiagaraComponent()->SetupAttachment(SphereComponent);
	GetNiagaraComponent()->SetWorldScale3D({ 0.9f, 0.9f, 0.9f });

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(SphereComponent);
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 2000.f;
	ProjectileMovement->ProjectileGravityScale = false;
}

USphereComponent* ADarkBall::GetSphereComponent()
{
	return SphereComponent;
}

void ADarkBall::ShotDarkBall(FVector _FVector)
{
	SetDeathCheck(true);

	if (nullptr == ProjectileMovement)
	{
		return;
	}

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = _FVector * ProjectileMovement->InitialSpeed;
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

