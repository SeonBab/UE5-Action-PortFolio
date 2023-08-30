#include "AI/Boss/Tornado.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "AI/Boss/SpellDecal.h"
#include "Global/GlobalGameInstance.h"

ATornado::ATornado()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetupAttachment(RootComponent);
	CapsuleComponent->SetCollisionProfileName("NoCollision", true);
	CapsuleComponent->SetCapsuleHalfHeight(300.f);
	CapsuleComponent->SetCapsuleRadius(130.f);

	GetNiagaraComponent()->SetupAttachment(CapsuleComponent);

	Speed = 0.f;
	DeathTime = 5.f;
	Damage = 2.f;
	HitTimeCheck = 1.f;
}

UCapsuleComponent* ATornado::GetCapsuleComponent()
{
	return CapsuleComponent;
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

	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	TSubclassOf<UObject> SpellDecal = Inst->GetSubClass(TEXT("SpellDecal"));

	if (nullptr == SpellDecal)
	{
		return;
	}

	ASpellDecal* SpawnSpellDecal = GetWorld()->SpawnActor<ASpellDecal>(SpellDecal);
	
	FVector SetPos({ this->GetActorLocation().X, this->GetActorLocation().Y, this->GetActorLocation().Z - CapsuleComponent->GetScaledCapsuleHalfHeight() });

	SpawnSpellDecal->SetActorLocation(SetPos);
	// 데칼 크기와 높이 변경
	// 일정 시간 후 사라지게 하기
	// 사라진 후 토네이도 만들어지게 하기
	// 충돌도 만들어진 후 하기
}

void ATornado::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	FPointDamageEvent DamageEvent;

	AController* Controller = GetCurController();

	if (nullptr == Controller)
	{
		return;
	}

	if (1.f > HitTimeCheck)
	{
		HitTimeCheck += DeltaTime;
	}
	else if (1.f <= HitTimeCheck && nullptr != OverlapOtherActor)
	{
		OverlapOtherActor->TakeDamage(Damage, DamageEvent, Controller, this);
		HitTimeCheck = 0.f;
	}

	if (nullptr != TargetCharacter)
	{
		// 느리게 따라가게 만들기
	}
}

