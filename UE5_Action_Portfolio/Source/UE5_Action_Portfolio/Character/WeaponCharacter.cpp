#include "Character/WeaponCharacter.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/WeaponData.h"
#include "Engine/DamageEvents.h"
#include "Weapon/WeaponAction.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"
#include "Character/MainCharacter.h"
#include "Components/CapsuleComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/CharacterMovementComponent.h"

AWeaponCharacter::AWeaponCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	UnArmedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UnArmedMesh"));
	BowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	SwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	ShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShiedlMesh"));

	UnArmedWeaponMesh->SetupAttachment(GetMesh(), TEXT("LeftHandSoket"));
	BowWeaponMesh->SetupAttachment(GetMesh(), TEXT("LeftHandSoket"));
	SwordWeaponMesh->SetupAttachment(GetMesh(), TEXT("RightHandSoket"));
	ShieldWeaponMesh->SetupAttachment(GetMesh(), TEXT("LeftHandShield"));

	UnArmedWeaponMesh->SetGenerateOverlapEvents(true);
	BowWeaponMesh->SetGenerateOverlapEvents(true);
	SwordWeaponMesh->SetGenerateOverlapEvents(true);

	BackBowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackBowMesh"));
	BackSwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackSwordMesh"));
	BackShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackShiedlMesh"));

	BackBowWeaponMesh->SetupAttachment(GetMesh(), TEXT("BackBow"));
	BackSwordWeaponMesh->SetupAttachment(GetMesh(), TEXT("BackSword"));
	BackShieldWeaponMesh->SetupAttachment(GetMesh(), TEXT("BackShield"));

	GetCapsuleComponent()->SetGenerateOverlapEvents(false);

	CurWeaponAction = nullptr;
}

UWeaponAction* AWeaponCharacter::GetCurWeaponAction()
{
	return CurWeaponAction;
}

void AWeaponCharacter::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 공격 충돌 처리
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	EWeaponType CurWeaponEnum = GetCurWeaponAction()->GetWeaponType();
	FName WeaponName;

	const UEnum* WeaponEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	if (0 != WeaponEnum)
	{
		FString WeaponString = WeaponEnum->GetNameStringByValue((int64)CurWeaponEnum);
		WeaponName = FName(WeaponString);
	}

	FWeaponData* WeaponData = Instance->GetWeaponData(WeaponName);

	int CurWeaponDamage = WeaponData->Damage;
	FPointDamageEvent DamageEvent;

	OtherActor->TakeDamage(CurWeaponDamage, DamageEvent, GetController(), this);
}

FVector AWeaponCharacter::GetBowJointLocation()
{
	if (nullptr == BowWeaponMesh)
	{
		return FVector::ZeroVector;
	}

	FVector Pos = BowWeaponMesh->GetSocketLocation(TEXT("Bow_04_Joint"));

	return Pos;
}

TTuple<float, FVector> AWeaponCharacter::IKFootLineTrace(FName _Socket, float _TraceDis)
{
	TTuple<float, FVector> ReturnValue;

	if (nullptr == GetCapsuleComponent())
	{
		return ReturnValue;
	}

	FVector SocketLocation = GetMesh()->GetSocketLocation(_Socket);
	FVector LineTraceStart = FVector(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z - 40.f);
	FVector LineTraceEnd = FVector(SocketLocation.X, SocketLocation.Y, GetActorLocation().Z - GetCapsuleComponent()->GetScaledCapsuleHalfHeight() - _TraceDis);

	FHitResult HitResult;
	TArray<AActor*> ToIgnore;

	ToIgnore.Emplace(GetOwner());

	bool DebugCheck = true;
	EDrawDebugTrace::Type DebugType = EDrawDebugTrace::None;
	
	if (true == DebugCheck)
	{
		DebugType = EDrawDebugTrace::ForOneFrame;
	}

	ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECC_Visibility);

	bool bResult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), LineTraceStart, LineTraceEnd, TraceType, true, ToIgnore, DebugType, HitResult, true);

	float ImpactLength;
	FVector ImpactLocation(HitResult.ImpactNormal);

	if (true == HitResult.IsValidBlockingHit())
	{
		ImpactLength = (HitResult.ImpactPoint - HitResult.TraceEnd).Size();
		ImpactLength -= _TraceDis;
		ImpactLength += 5.f;
	}
	else
	{
		ImpactLength = 0.f;
	}


	ReturnValue.Key = ImpactLength;
	ReturnValue.Value = ImpactLocation;

	return ReturnValue;
}

FRotator AWeaponCharacter::NormalToRotator(FVector _Vector)
{
	float FootAtan2_1 = UKismetMathLibrary::DegAtan2(_Vector.Y, _Vector.Z);
	float FootAtan2_2 = UKismetMathLibrary::DegAtan2(_Vector.X, _Vector.Z);
	
	// 값을 음수로
	FootAtan2_2 *= -1.f;

	FRotator ReturnRotator = FRotator(FootAtan2_2, 0.0f, FootAtan2_1);

	return ReturnRotator;
}

void AWeaponCharacter::UpdateFootRotation(float _DeltaTime, FRotator _NormalToRotatorValue, FRotator* _FootRotatorValue, float _InterpSpeed)
{
	FRotator InterpRotator = UKismetMathLibrary::RInterpTo(*_FootRotatorValue, _NormalToRotatorValue, _DeltaTime, _InterpSpeed);
	*_FootRotatorValue = InterpRotator;
}

void AWeaponCharacter::UpdateCapsuleHalfHeight(float _DeltaTime, float _HipsShifs, bool _ResetDefault)
{
	if (nullptr == GetCapsuleComponent())
	{
		return;
	}

	float CapsuleHalf = 0.0f;

	if (true == _ResetDefault)
	{
		CapsuleHalf = CurCapsuleSize;
	}
	else
	{
		float HalfAbsSize = UKismetMathLibrary::Abs(_HipsShifs);
		CapsuleHalf = CurCapsuleSize - HalfAbsSize;
	}

	float InterpValue = UKismetMathLibrary::FInterpTo(GetCapsuleComponent()->GetScaledCapsuleHalfHeight(), CapsuleHalf, _DeltaTime, 13.f);

	GetCapsuleComponent()->SetCapsuleHalfHeight(InterpValue, true);
}

void AWeaponCharacter::UpdateFootOffset(float _DeltaTime, float _TargetValue, float* _EffectorValue, float _InterpSpeed)
{
	float InterpValue = UKismetMathLibrary::FInterpTo(*_EffectorValue, _TargetValue, _DeltaTime, _InterpSpeed);
	*_EffectorValue = InterpValue;
}

void AWeaponCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	CurWeaponAction = NewObject<UWeaponAction>();
	CurWeaponAction->SetCurCharacter(this);

	UGlobalGameInstance* Instance = GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));

	BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
	BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
	BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));

	UnArmedWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponCharacter::WeaponBeginOverlap);
	BowWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponCharacter::WeaponBeginOverlap);
	SwordWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &AWeaponCharacter::WeaponBeginOverlap);

	GetCurWeaponAction()->SetAttackType(GetAttackTypeTag());

	CurCapsuleSize = GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
}

void AWeaponCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	UWeaponAction* CurWeapon = GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	CurWeapon->Tick(_DeltaTime);

	if (EWeaponType::Bow == CurWeapon->GetWeaponType())
	{
		BowChordMove();
	}
	CharacterAnimState CurState = CurWeapon->GetAnimState();

	if (CharacterAnimState::WalkJump != CurState && CharacterAnimState::RunJump != CurState)
	{
		TTuple<float, FVector> LeftTrace = IKFootLineTrace(TEXT("LeftFoot"), 55.f);
		TTuple<float, FVector> RightTrace = IKFootLineTrace(TEXT("RightFoot"), 55.f);

		UpdateFootRotation(_DeltaTime, NormalToRotator(LeftTrace.Get<1>()), &FootRotatorLeft, 20.f);
		UpdateFootRotation(_DeltaTime, NormalToRotator(RightTrace.Get<1>()), &FootRotatorRight, 20.f);

		float HipOffsetValue = UKismetMathLibrary::Min(LeftTrace.Get<0>(), RightTrace.Get<0>());

		if (0.f < HipOffsetValue)
		{
			HipOffsetValue = 0.f;
		}

		UpdateFootOffset(_DeltaTime, HipOffsetValue, &HipOffset, 20.f);
		UpdateCapsuleHalfHeight(_DeltaTime, HipOffsetValue, false);

		UpdateFootOffset(_DeltaTime, LeftTrace.Get<0>() - HipOffsetValue, &FootOffsetLeft, 20.f);
		UpdateFootOffset(_DeltaTime, RightTrace.Get<0>() - HipOffsetValue, &FootOffsetRight, 20.f);
	}
}

void AWeaponCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

float AWeaponCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float FinalDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	// PointDamage를 전달 받았다.
	if (DamageEvent.IsOfType(FPointDamageEvent::ClassID))
	{
		if (nullptr == EventInstigator)
		{
			return 0.f;
		}

		FVector HitDir = EventInstigator->GetPawn()->GetActorLocation();
		HitDir.Z = 0;

		FVector CurPos = GetActorLocation();
		CurPos.Z = 0;

		FVector Dir = HitDir - CurPos;
		Dir.Normalize();

		FVector CurForward = GetActorForwardVector();
		CurForward.Normalize();

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = CurForward.Rotation().Yaw;

		bool BlockCheck = GetCurWeaponAction()->GetIsBlock();
		bool ParryCheck = GetCurWeaponAction()->GetIsParry();
		bool IsInvincibilityCheck = GetCurWeaponAction()->GetIsInvincibility();

		if (160.f >= FMath::Abs(Angle0 - Angle1) && true == BlockCheck)
		{
			FinalDamage *= 0.1f;
		}
		else if (true == IsInvincibilityCheck)
		{
			FinalDamage = 0.f;

			return FinalDamage;
		}
		else if (160.f >= FMath::Abs(Angle0 - Angle1) && true == ParryCheck)
		{
			FinalDamage = 0.f;

			AWeaponCharacter* EnemyChar = Cast<AWeaponCharacter>(EventInstigator->GetPawn());

			UWeaponAction* EnemyWeaponAction = EnemyChar->GetCurWeaponAction();

			EnemyWeaponAction->ChangeNoCollision();
			// 애니메이션이 정상적으로 재생 안됨
			EnemyWeaponAction->SetAnimState(CharacterAnimState::Dizzy);
			
			return FinalDamage;
		}

		if (0.f < GetHP() && 0.f < FinalDamage)
		{
			SetHP(GetHP() - FinalDamage);
		}
		
		if (0.f < GetHP())
		{
			// 생존
			GetCurWeaponAction()->GotHit(Dir);
		}
		else if (0.f >= GetHP())
		{
			// 죽음
			GetCapsuleComponent()->SetCollisionProfileName(TEXT("NoCollision"), true);
			GetMesh()->SetCollisionProfileName(TEXT("NoCollision"), true);

			GetCurWeaponAction()->Death();

			if (nullptr == EventInstigator)
			{
				return 0.f;
			}

			bool PlayerCheck = EventInstigator->GetPawn()->ActorHasTag("Player");
			
			if (true == PlayerCheck)
			{
				AMainCharacter* MainChar = Cast<AMainCharacter>(EventInstigator->GetPawn());

				if (nullptr != MainChar)
				{
					MainChar->LostLockedOnTargetActor();
				}
			}
			// 체력이 음수값이 되지 않게
			SetHP(0.f);

			return FinalDamage;
		}
	}
	
	return FinalDamage;
}

void AWeaponCharacter::BowChordMove()
{	
	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

	if (nullptr == BowAnim)
	{
		return;
	}

	if (true == BowAnim->GetBowChordCheck())
	{
		USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

		if (nullptr != SkeletalMeshComponent)
		{
			FVector Vec = SkeletalMeshComponent->GetSocketLocation(TEXT("RightHandSoket"));

			BowAnim->SetHandTransform(Vec);
		}
	}
}