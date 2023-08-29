#include "AI/Boss/Lich.h"
#include "Global/GlobalGameInstance.h"
#include "AI/Boss/Enums_Boss.h"
#include "Engine/DamageEvents.h"
#include "BehaviorTree/BlackboardComponent.h"

ALich::ALich()
{
	PrimaryActorTick.bCanEverTick = true;

	SetActorTypeTag(TEXT("Monster"));
	SetAttackTypeTag(TEXT("MonsterAttack"));
}

void ALich::SetDarkBall(AActor* _Actor)
{
	DarkBall = _Actor;
}

AActor* ALich::GetDarkBall()
{
	return DarkBall;
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

}

void ALich::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALich::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

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

