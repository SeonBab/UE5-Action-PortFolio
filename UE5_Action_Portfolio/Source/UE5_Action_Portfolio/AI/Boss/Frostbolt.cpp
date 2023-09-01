#include "AI/Boss/Frostbolt.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Global/GlobalGameInstance.h"

AFrostbolt::AFrostbolt()
{
	Speed = 4000.f;
	DeathTime = 3.f;
	Damage = 10.f;
	Shot = false;

	TargetActor = nullptr;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(RootComponent);
	SphereComponent->SetCollisionProfileName(TEXT("NoCollision"));

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(SphereComponent);
	ParticleComponent->SetWorldScale3D({ 2.f, 2.f, 2.f });

	ShotParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ShotParticleComponent"));
	ShotParticleComponent->SetupAttachment(SphereComponent);
	ShotParticleComponent->SetWorldScale3D({ 1.f, 1.5f, 1.5f });

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovement->SetUpdatedComponent(SphereComponent);
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 5000.f;
	ProjectileMovement->ProjectileGravityScale = false;

	GetNiagaraComponent()->SetupAttachment(SphereComponent);
}

USphereComponent* AFrostbolt::GetSphereComponent()
{
	return SphereComponent;
}

void AFrostbolt::SetTargetActor(AActor* _TargetActor)
{
	if (nullptr == _TargetActor)
	{
		return;
	}

	TargetActor = _TargetActor;
}

void AFrostbolt::FrostboltBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
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

void AFrostbolt::ParticleSystemFinished(UParticleSystemComponent* _ParticleSystemComponent)
{
	// 고드름 파티클로 생성
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	UParticleSystem* FrostboltAttackParticle = Inst->GetParticleAsset(TEXT("FrostboltAttack"));

	if (nullptr == FrostboltAttackParticle)
	{
		return;
	}

	ShotParticleComponent->SetTemplate(FrostboltAttackParticle);
}

void AFrostbolt::BeginPlay()
{
	Super::BeginPlay();

	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AFrostbolt::FrostboltBeginOverlap);
	ParticleComponent->OnSystemFinished.AddDynamic(this, &AFrostbolt::ParticleSystemFinished);
	OnDestroyed.AddDynamic(this, &AFrostbolt::DestroyProjectile);
}

void AFrostbolt::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (false == Shot)
	{
		AimTarget(DeltaTime);
	}
}

void AFrostbolt::DestroyProjectile(AActor* _Destroy)
{
	if (nullptr == DeathCreateObject)
	{
		return;
	}

	AActor* Actor = GetWorld()->SpawnActor<AActor>(DeathCreateObject);

	Actor->SetActorLocation(GetActorLocation());
	Actor->SetActorRotation(GetActorRotation());
}

void AFrostbolt::AimTarget(float _DeltaTime)
{
	// 발사 전까지 타겟을 향해 방향을 돌림
	if (nullptr == TargetActor)
	{
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = this->GetActorLocation();

	FVector Dir = TargetPos - CurPos;
	Dir.Normalize();

	FVector OtherForward = this->GetActorForwardVector();
	OtherForward.Normalize();

	FVector Cross = FVector::CrossProduct(OtherForward, Dir);

	float Angle0 = Dir.Rotation().Yaw;
	float Angle1 = OtherForward.Rotation().Yaw;

	if (5.f <= FMath::Abs(Angle0 - Angle1))
	{
		FRotator Rot = FRotator::MakeFromEuler({ Cross.Z * 500.f * _DeltaTime, Cross.X * 500.f * _DeltaTime, Cross.Z * 500.f * _DeltaTime });
		this->AddActorWorldRotation(Rot);
	}
	else
	{
		FRotator Rot = Dir.Rotation();
		this->SetActorRotation(Rot);
	}
}

void AFrostbolt::ShotFrostbolt()
{
	SetDeathCheck(true);

	if (nullptr == TargetActor)
	{
		return;
	}

	FVector TargetPos = TargetActor->GetActorLocation();
	FVector CurPos = this->GetActorLocation();

	FVector Dir = TargetPos - CurPos;
	Dir.Normalize();

	if (nullptr == ProjectileMovement)
	{
		return;
	}

	ProjectileMovement->InitialSpeed = Speed;
	ProjectileMovement->Velocity = Dir * ProjectileMovement->InitialSpeed;
}
