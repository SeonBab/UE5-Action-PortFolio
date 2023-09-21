#include "AI/Boss/DarkBall.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

ADarkBall::ADarkBall()
{
	PrimaryActorTick.bCanEverTick = true;

	Speed = 6000.f;
	DeathTime = 3.f;
	Damage = 10.f;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName("NoCollision", true);
	SphereComponent->SetWorldScale3D({ 1.3f, 1.3f, 1.3f });

	GetNiagaraComponent()->SetupAttachment(SphereComponent);
	GetNiagaraComponent()->SetWorldScale3D({ 1.f, 1.f, 1.f });

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->SetUpdatedComponent(SphereComponent);
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 10000.f;
	ProjectileMovement->ProjectileGravityScale = false;
}

USphereComponent* ADarkBall::GetSphereComponent()
{
	return SphereComponent;
}

void ADarkBall::ShotDarkBall(FVector _FVector)
{
	SetDeathCheck(true);

	if (false == IsValid(ProjectileMovement))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = _FVector * ProjectileMovement->InitialSpeed;
}

void ADarkBall::DarkBallBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FPointDamageEvent DamageEvent;

	AController* Controller = GetCurController();

	if (false == IsValid(Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
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
	if (false == IsValid(DeathCreateObject))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor<AActor>(DeathCreateObject);

	if (false == IsValid(Actor))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	Actor->SetActorLocation(GetActorLocation());
	Actor->SetActorRotation(GetActorRotation());
}

