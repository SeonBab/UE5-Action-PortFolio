#include "AI/Boss/Tornado.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/DecalComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Global/GlobalGameInstance.h"

ATornado::ATornado()
{
	PrimaryActorTick.bCanEverTick = true;

	TornadoSpawnCheck = false;

	DecalFadeStartDelay = 1.f;
	DecalFadeDuration = 1.f;

	Speed = 100.f;
	DeathTime = 5.f;
	Damage = 2.f;
	HitTime = 1.f;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCollisionProfileName("NoCollision", true);
	CapsuleComponent->SetCapsuleHalfHeight(300.f);
	CapsuleComponent->SetCapsuleRadius(40.f);
	
	GetNiagaraComponent()->SetupAttachment(CapsuleComponent);

	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(CapsuleComponent);
	DecalComponent->DecalSize.Set(0.6f, CapsuleComponent->GetScaledCapsuleRadius(), CapsuleComponent->GetUnscaledCapsuleRadius());
	DecalComponent->FadeStartDelay = 1.f;
	DecalComponent->FadeDuration = 1.f;
	DecalComponent->bDestroyOwnerAfterFade = false;	
}

UCapsuleComponent* ATornado::GetCapsuleComponent()
{
	return CapsuleComponent;
}

void ATornado::SetAttackType(FName _AttackType)
{
	AttackType = _AttackType;
}

void ATornado::SetTargetCharacter(ACharacter* _TargetCharacter)
{
	if (nullptr == _TargetCharacter)
	{
		return;
	}

	TargetCharacter = _TargetCharacter;
}

void ATornado::TornadoBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	OverlapOtherActor = OtherActor;
}

void ATornado::TornadoEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == OverlapOtherActor)
	{
		OverlapOtherActor = nullptr;
	}
}

void ATornado::BeginPlay()
{
	Super::BeginPlay();
	
	CapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ATornado::TornadoBeginOverlap);
	CapsuleComponent->OnComponentEndOverlap.AddDynamic(this, &ATornado::TornadoEndOverlap);
}

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if ((DecalFadeStartDelay + DecalFadeDuration) <= DecalTime && false == TornadoSpawnCheck)
	{
		// 나이아가라 설정
		UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

		if (nullptr != Inst)
		{
			UNiagaraSystem* Storm = Inst->GetNiagaraAsset(TEXT("Storm"));

			if (nullptr != Storm)
			{
				UNiagaraComponent* CurNiagaraComponent = GetNiagaraComponent();

				if (nullptr != CurNiagaraComponent)
				{
					CurNiagaraComponent->SetAsset(Storm);
				}
			}
		}
		
		// 콜리전 변경
		CapsuleComponent->SetCollisionProfileName(AttackType);

		SetDeathCheck(true);
		TornadoSpawnCheck = true;
	}
	else if ((DecalFadeStartDelay + DecalFadeDuration) > DecalTime)
	{
		DecalTime += DeltaTime;
	}

	AController* Controller = GetCurController();

	if (nullptr != Controller)
	{
		FPointDamageEvent DamageEvent;
		
		if (1.f > HitTime)
		{
			HitTime += DeltaTime;
		}
		else if (1.f <= HitTime && nullptr != OverlapOtherActor)
		{
			OverlapOtherActor->TakeDamage(Damage, DamageEvent, Controller, this);
			HitTime = 0.f;
		}
	}

	if (nullptr != TargetCharacter && true == TornadoSpawnCheck)
	{
		// 타겟 느리게 따라가기
		FVector TargetPos = TargetCharacter->GetActorLocation();
		FVector CurPos = this->GetActorLocation();

		TargetPos.Z = 0.f;
		CurPos.Z = 0.f;

		FVector Dis = TargetPos - CurPos;
		
		if (10.f <= Dis.Size())
		{
			Dis.Normalize();
			AddActorWorldOffset(Dis * Speed * DeltaTime, true);
		}
	}
}

