#include "AI/Boss/Lich.h"
#include "AI/Boss/Enums_Boss.h"
#include "Global/GlobalGameInstance.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Components/CapsuleComponent.h"
#include "BehaviorTree/BlackboardComponent.h"

ALich::ALich()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTypeTag(TEXT("Monster"));
	SetAttackTypeTag(TEXT("MonsterAttack"));

	MeleeDamage = 15.f;

	MeleeCapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	MeleeCapsuleComponent->SetupAttachment(GetMesh(), TEXT("MeleeSocket"));
	MeleeCapsuleComponent->SetCollisionProfileName("NoCollision", true);
	MeleeCapsuleComponent->SetCapsuleHalfHeight(33.f);
	MeleeCapsuleComponent->SetCapsuleRadius(10.f);

	NiagaraComponent = CreateDefaultSubobject<UNiagaraComponent>(TEXT("NiagaraComponent"));
	NiagaraComponent->SetupAttachment(MeleeCapsuleComponent);

	UCapsuleComponent* CurCapsuleComponent = GetCapsuleComponent();

	if (nullptr != CurCapsuleComponent)
	{
		FName ActorType = GetActorTypeTag();

		CurCapsuleComponent->SetCollisionProfileName(ActorType);
	}
}

void ALich::SetDarkBall(AActor* _Actor)
{
	DarkBall = _Actor;
}

AActor* ALich::GetDarkBall()
{
	return DarkBall;
}

UCapsuleComponent* ALich::GetMeleeCapsuleComponent()
{
	return MeleeCapsuleComponent;
}

UNiagaraComponent* ALich::GetNiagaraComponent()
{
	return NiagaraComponent;
}

void ALich::Destroyed()
{
	if (DarkBall != nullptr)
	{
		DarkBall->Destroyed();
	}

	Super::Destroyed();
}

void ALich::BeginPlay()
{
	Super::BeginPlay();
	
	UGlobalGameInstance* Inst = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr != Inst)
	{
		CurBossData = Inst->GetBossData(DataName);

		SetAllAnimation<BossAnimState>(CurBossData->MapAnimation);
		SetHP(CurBossData->HP);
		SetMaxHP(GetHP());
		SetAnimState(BossAnimState::Idle);
	}

	GetBlackboardComponent()->SetValueAsObject(TEXT("TargetActor"), nullptr);
	MeleeCapsuleComponent->OnComponentBeginOverlap.AddDynamic(this, &ALich::MeleeBeginOverlap);
}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALich::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void ALich::MeleeBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	FPointDamageEvent DamageEvent;

	OtherActor->TakeDamage(MeleeDamage, DamageEvent, GetController(), this);
}

float ALich::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage를 전달 받았다.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (nullptr == EventInstigator)
		{
			return 0.f;
		}

		if (0.f < GetHP() && 0.f < FinalDamage)
		{
			SetHP(GetHP() - FinalDamage);
		}

		if (0.f < GetHP())
		{
			// 생존
			//GetCurWeaponAction()->GotHit(Dir);
		}
		else if (0.f >= GetHP())
		{
			// 죽음
		}

		// 체력이 음수값이 되지 않게
		SetHP(0.f);

		return FinalDamage;
	}

	return FinalDamage;
}

