#include "Weapon/Arrow.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"
#include "Global/Data/WeaponData.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"

AArrow::AArrow()
{
	PrimaryActorTick.bCanEverTick = true;

	InitialLifeSpan = 0.f;
	IsLocationAndRotation = true;
	Shot = false;
	ShotTime = 0.f;

	ArrowScene = CreateDefaultSubobject<USceneComponent>(TEXT("Arrowcene"));
	SetRootComponent(ArrowScene);

	ArrowSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ArrowMesh"));
	ArrowSkeletalMesh->SetupAttachment(ArrowScene);
	ArrowSkeletalMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
	ArrowSkeletalMesh->SetRelativeScale3D({ 2.f, 2.f, 1.5f });

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->InitialSpeed = 0.f;
	ProjectileMovement->MaxSpeed = 10000.f;

	ParticleComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleComponent"));
	ParticleComponent->SetupAttachment(ArrowSkeletalMesh);
	ParticleComponent->AddLocalOffset({ 0.f, 0.f, 20.f });
	ParticleComponent->SetWorldScale3D({ 0.05f, 0.05f, 0.3f });
	ParticleComponent->CustomTimeDilation = 2.f;

}

void AArrow::ArrowBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 공격 충돌 처리
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FWeaponData* WeaponData = Instance->GetWeaponData(TEXT("Arrow"));

	float CurWeaponDamage = WeaponData->Damage;
	FPointDamageEvent DamageEvent;

	OtherActor->TakeDamage(CurWeaponDamage, DamageEvent, CurController, this);

	Destroy();
}

void AArrow::ArrowReRoad(ACharacter* _Character, FVector _JointPos, float _DeltaTime)
{
	if (IsLocationAndRotation == true)
	{
		FVector SetPos = _Character->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
		
		SetActorLocation(SetPos);

		FVector Dir = _JointPos - SetPos;
		Dir.Normalize();

		SetActorRotation(Dir.Rotation());
	}
}

void AArrow::SetIsLocationAndRotation(bool _Value)
{
	IsLocationAndRotation = _Value;
}

void AArrow::ArrowChangeCollision(FName _FName)
{
	ArrowSkeletalMesh->SetCollisionProfileName(_FName, true);
}

void AArrow::FireInDirection(FVector _FVector, FRotator _FRotator, AController* _Controller)
{
	if (false == IsValid(_Controller))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CurController = _Controller;

	ProjectileMovement->SetUpdatedComponent(ArrowSkeletalMesh);
	ProjectileMovement->InitialSpeed = 10000.f;

	Shot = true;
	SetActorRotation(_FRotator);
	AddActorLocalOffset({ 0.f, 0.f, 15.f });
	ProjectileMovement->Velocity = _FVector * ProjectileMovement->InitialSpeed;

	ArrowSkeletalMesh->SetGenerateOverlapEvents(true);


	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	 UParticleSystem* ParticleSystem = Instance->GetParticleAsset(TEXT("ArrowIceCircle"));

	 if (nullptr == ParticleSystem || false == ParticleSystem->IsValidLowLevel())
	 {
		 UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		 return;
	 }

	ParticleComponent->SetTemplate(ParticleSystem);
}

void AArrow::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	ArrowSkeletalMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Arrow")));

	ArrowSkeletalMesh->OnComponentBeginOverlap.AddDynamic(this, &AArrow::ArrowBeginOverlap);
}

void AArrow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (true == Shot)
	{
		ShotTime += DeltaTime;

		if (3.f <= ShotTime)
		{
			Destroy();
		}
	}

}

