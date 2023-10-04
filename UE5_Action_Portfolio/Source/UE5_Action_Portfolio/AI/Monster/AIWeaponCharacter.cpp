#include "AI/Monster/AIWeaponCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Weapon/WeaponComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Engine/DamageEvents.h"

AAIWeaponCharacter::AAIWeaponCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SetAnimState<CharacterAnimState>(CharacterAnimState::Idle);

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);
	float CapsuleSize = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
	CurCapsuleSize = CapsuleSize;

	FootIKComponent = CreateDefaultSubobject<UFootIKComponent>(TEXT("FootIK"));
	FootIKComponent->SetBeginCapsuleSize(CapsuleSize);
	FootIKComponent->SetTraceDis(45.f);
	FootIKComponent->SetFootSoket(TEXT("LeftFoot"), TEXT("RightFoot"));

	WeaponComponent = CreateDefaultSubobject<UWeaponComponent>(TEXT("Weapon"));
	WeaponComponent->SetMeshAttach(GetMesh());
}

UWeaponComponent* AAIWeaponCharacter::GetWeaponComponent()
{
	return WeaponComponent;
}

void AAIWeaponCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AAIWeaponCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	CharacterAnimState CurState = static_cast<CharacterAnimState>(GetAnimState());
	bool IsFall = GetMovementComponent()->IsFalling();

	// 공중에 있으면 본의 위치를 변경 하지 않는다
	if ((CharacterAnimState::WalkJump != CurState && CharacterAnimState::RunJump != CurState && CharacterAnimState::Roll != CurState) && false == IsFall)
	{
		FootIKOffset = FootIKComponent->GetFootIKOffset(this, _DeltaTime);

		TTuple<float, FVector> LeftTrace = FootIKComponent->FootIKLineTrace(this, GetCapsuleComponent(), TEXT("LeftFoot"), 45.f);
		TTuple<float, FVector> RightTrace = FootIKComponent->FootIKLineTrace(this, GetCapsuleComponent(), TEXT("RightFoot"), 45.f);

		//UpdateFootRotation(_DeltaTime, NormalToRotator(LeftTrace.Get<1>()), &FootRotatorLeft, 20.f);
		//UpdateFootRotation(_DeltaTime, NormalToRotator(RightTrace.Get<1>()), &FootRotatorRight, 20.f);
		//FootRotatorLeft = FootIKOffset.FootIKRotatorLeft;
		//FootRotatorRight = FootIKOffset.FootIKRotatorRight;
		FootIKComponent->UpdateFootRotation(_DeltaTime, FootIKComponent->NormalToRotator(LeftTrace.Get<1>()), &FootRotatorLeft, 20.f);
		FootIKComponent->UpdateFootRotation(_DeltaTime, FootIKComponent->NormalToRotator(RightTrace.Get<1>()), &FootRotatorRight, 20.f);

		float HipOffsetValue = UKismetMathLibrary::Min(LeftTrace.Get<0>(), RightTrace.Get<0>());

		if (0.f < HipOffsetValue)
		{
			HipOffsetValue = 0.f;
		}

		FootIKComponent->UpdateFootOffset(_DeltaTime, HipOffsetValue, &HipOffset, 20.f);
		FootIKComponent->UpdateCapsuleHalfHeight(GetCapsuleComponent(), _DeltaTime, HipOffsetValue, false);

		FootIKComponent->UpdateFootOffset(_DeltaTime, LeftTrace.Get<0>() - HipOffsetValue, &FootOffsetLeft, 20.f);
		FootIKComponent->UpdateFootOffset(_DeltaTime, RightTrace.Get<0>() - HipOffsetValue, &FootOffsetRight, 20.f);
	}
}

float AAIWeaponCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage를 전달 받았다.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (false == IsValid(EventInstigator))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		APawn* EventInstigatorPawn = EventInstigator->GetPawn();

		if (false == IsValid(EventInstigatorPawn))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		UWeaponComponent* CurWeaponComponent = GetWeaponComponent();

		if (false == IsValid(CurWeaponComponent))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		bool IsRoll = CurWeaponComponent->GetIsRollMove();

		if (true == IsRoll)
		{
			return 0.f;
		}

		FVector HitDir = EventInstigatorPawn->GetActorLocation();
		HitDir.Z = 0;

		FVector CurPos = GetActorLocation();
		CurPos.Z = 0;

		FVector Dir = HitDir - CurPos;
		Dir.Normalize();

		FVector CurForward = GetActorForwardVector();
		CurForward.Normalize();

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = CurForward.Rotation().Yaw;

		bool BlockCheck = CurWeaponComponent->GetIsBlock();
		bool ParryCheck = CurWeaponComponent->GetIsParry();
		bool IsInvincibilityCheck = GetIsInvincibility();

		AGlobalCharacter* GlobalChar = Cast<AGlobalCharacter>(EventInstigatorPawn);

		if (false == IsValid(GlobalChar))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return 0.f;
		}

		bool EnemyParrybool = GlobalChar->GetParrybool();

		if (160.f >= FMath::Abs(Angle0 - Angle1) && true == BlockCheck)
		{
			// 가드
			FinalDamage *= 0.1f;
		}
		else if (true == IsInvincibilityCheck)
		{
			// 무적
			FinalDamage = 0.f;

			return FinalDamage;
		}
		else if (160.f >= FMath::Abs(Angle0 - Angle1) && (true == ParryCheck && true == EnemyParrybool))
		{
			// 패링
			FinalDamage = 0.f;

			AAIWeaponCharacter* EnemyChar = Cast<AAIWeaponCharacter>(GlobalChar);

			if (false == IsValid(EnemyChar))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return 0.f;
			}

			UWeaponComponent* EnemyWeaponComponent = EnemyChar->GetWeaponComponent();

			if (false == IsValid(EnemyWeaponComponent))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return 0.f;
			}

			EnemyWeaponComponent->OverlapEnd();

			EnemyChar->SetAnimState(CharacterAnimState::Dizzy);

			return FinalDamage;
		}

		if (0.f < GetHP() && 0.f < FinalDamage)
		{
			SetHP(GetHP() - FinalDamage);
		}

		if (0.f < GetHP())
		{
			// 생존
			CurWeaponComponent->GotHit(Dir);
		}
		else if (0.f >= GetHP())
		{
			//죽음
			
			// 체력은 음수값이 되지하지 않아야한다.
			SetHP(0.f);

			// 플레이어 캐릭터는 죽으면 안되므로 죽음 처리는 각자 자식이 처리한다.

			return FinalDamage;
		}
	}

	return FinalDamage;
}
