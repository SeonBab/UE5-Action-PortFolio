#include "Character/MainCharAnimInstance.h"
#include "Global/Data/AnimaitionData.h"
#include "Global/GlobalGameInstance.h"
#include "Character/WeaponCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/WeaponAction.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"
#include "Character/MainCharacter.h"

void UMainCharAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;

	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	if (CharacterAnimState::Death == CurWeapon->GetAnimState())
	{
		return;
	}
	else if (CharacterAnimState::Dizzy == CurWeapon->GetAnimState() && GetAnimMontage(CharacterAnimState::Dizzy) != Anim)
	{
		return;
	}


	if (GetAnimMontage(CharacterAnimState::WalkJump) == Anim || GetAnimMontage(CharacterAnimState::RunJump) == Anim ||
		GetAnimMontage(CharacterAnimState::Roll) == Anim || GetAnimMontage(CharacterAnimState::Attack) == Anim ||
		GetAnimMontage(CharacterAnimState::GotHit) == Anim || GetAnimMontage(CharacterAnimState::Dizzy) == Anim)
	{
		if (false == CurWeapon->GetIsLockOn())
		{
			CurWeapon->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == CurWeapon->GetIsLockOn())
		{
			CurWeapon->SetAnimState(CharacterAnimState::LockOnIdle);
		}
		Montage_Play(AllAnimations[CurWeapon->GetAnimStateToInt()], 1.f);
		CurWeapon->SetCharacterAirControl(1.f);
	}
	else if (GetAnimMontage(CharacterAnimState::EquipOrDisArmBow) == Anim || GetAnimMontage(CharacterAnimState::EquipOrDisArmSwordAndShield) == Anim)
	{
		if (false == CurWeapon->GetIsLockOn())
		{
			CurWeapon->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == CurWeapon->GetIsLockOn())
		{
			CurWeapon->SetAnimState(CharacterAnimState::LockOnIdle);
		}
		Montage_Play(AllAnimations[CurWeapon->GetAnimStateToInt()], 1.f);
		AnimSpeed = 1.f;

		if (true == CurWeapon->SwordAndSheiledToBow)
		{
			CurWeapon->ChangeSetBow();
			CurWeapon->SwordAndSheiledToBow = false;
		}
		else if (true == CurWeapon->BowToSwordAndSheiled)
		{
			CurWeapon->ChangeSetSwordAndSheiled();
			CurWeapon->BowToSwordAndSheiled = false;
		}
	}
	else if (GetAnimMontage(CharacterAnimState::ParryorFire) == Anim)
	{
		if (EWeaponType::Bow == character->CurWeaponAction->WeaponType)
		{
			CurWeapon->SetAnimState(CharacterAnimState::Idle);
			Montage_Play(AllAnimations[CurWeapon->GetAnimStateToInt()], 1.f);
			CurWeapon->ArrowReady = false;
			CurWeapon->EarlyArrowCheck = false;
		}
		else if (EWeaponType::Sword == CurWeapon->GetWeaponType())
		{
			if (false == CurWeapon->GetIsLockOn())
			{
				CurWeapon->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == CurWeapon->GetIsLockOn())
			{
				CurWeapon->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			Montage_Play(AllAnimations[CurWeapon->GetAnimStateToInt()], 1.f);
			CurWeapon->SetCharacterAirControl(1.f);
		}
	}
	else if (GetAnimMontage(CharacterAnimState::AimOrBlockGotHit) == Anim)
	{
		if (EWeaponType::Sword == character->GetCurWeaponAction()->GetWeaponType())
		{
			CurWeapon->SetAnimState(CharacterAnimState::AimOrBlock);

			// 다시 블락
			FName SectionName = "AimorBlock2";

			UAnimMontage* Montage = AllAnimations[CurWeapon->GetAnimStateToInt()];

			if (nullptr == Montage)
			{
				return;
			}

			Montage_JumpToSection(SectionName, Montage);
		}
		else if (false == character->CurWeaponAction->IsLockOn)
		{
			CurWeapon->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == character->CurWeaponAction->IsLockOn)
		{
			CurWeapon->SetAnimState(CharacterAnimState::LockOnIdle);
		}
	}
}

void UMainCharAnimInstance::AnimNotify_RollStop()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->CurWeaponAction->IsRollMoveToFalse();
}

void UMainCharAnimInstance::AnimNotify_JumpStart()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->Jump();
}

void UMainCharAnimInstance::AnimNotify_ChangeWeapon()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	EWeaponType CurWeaponType = character->GetCurWeaponAction()->GetWeaponType();
	CharacterAnimState CurAnimState = character->GetCurWeaponAction()->GetAnimState();

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	// 전환 하려는 무기가 맨손일 때
	if (EWeaponType::UnArmed == CurWeaponType)
	{
		// 활을 들고 있을 때
		if (CharacterAnimState::EquipOrDisArmBow == CurAnimState)
		{
			character->BowWeaponMesh->SetSkeletalMesh(nullptr);
			character->BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
			character->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}
		// 칼과 방패를 들고 있을 때
		else if (CharacterAnimState::EquipOrDisArmSwordAndShield == CurAnimState)
		{
			character->SwordWeaponMesh->SetSkeletalMesh(nullptr);
			character->ShieldWeaponMesh->SetSkeletalMesh(nullptr);
			character->BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
			character->BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
			character->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}

	}
	// 전환 하려는 무기가 활일 때
	else if (EWeaponType::Bow == CurWeaponType)
	{
		character->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackBowWeaponMesh->SetSkeletalMesh(nullptr);
		character->BowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));

	}
	// 전환 하려는 무기가 칼과 방패일 때
	else if (EWeaponType::Sword == CurWeaponType)
	{
		character->SwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
		character->ShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
		character->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackSwordWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackShieldWeaponMesh->SetSkeletalMesh(nullptr);
	}
}

void UMainCharAnimInstance::AnimNotify_AttackCheck()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	bool Value = CurWeapon->GetAttackCheck();

	if (CharacterAnimState::Attack == CurWeapon->GetAnimState())
	{
		if (false == Value)
		{
			if (false == CurWeapon->GetIsLockOn())
			{
				CurWeapon->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == CurWeapon->GetIsLockOn())
			{
				CurWeapon->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			Montage_Play(AllAnimations[CurWeapon->GetAnimStateToInt()], 1.0f);
		}
		else
		{
			CurWeapon->SetAttackCheck(false);
		}
	}
}

void UMainCharAnimInstance::AnimNotify_AimorBlockCheck()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	if (CharacterAnimState::AimOrBlock == CurWeapon->GetAnimState())
	{
		// 다시 에임or 블락
		FName SectionName = "AimorBlock2";

		UAnimMontage* Montage = AllAnimations[CurWeapon->GetAnimStateToInt()];

		if (nullptr == Montage)
		{
			return;
		}

		Montage_JumpToSection(SectionName, Montage);
	}
}

void UMainCharAnimInstance::AnimNotify_ArrowReadyCheck()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	// 발사가 준비되기 전에 미리 공격 입력을 눌렀을 때
	if (true == CurWeapon->EarlyArrowCheck && CharacterAnimState::AimOrBlock == CurWeapon->GetAnimState())
	{
		CurWeapon->ArrowReady = false;
		CurWeapon->EarlyArrowCheck = false;

		CurWeapon->SetAnimState(CharacterAnimState::ParryorFire);
		Montage_Play(AllAnimations[CurWeapon->GetAnimStateToInt()], 1.f);
	}
	// 발사가 준비된 후 입력을 확인할 때
	else if (false == CurWeapon->EarlyArrowCheck && CharacterAnimState::AimOrBlock == CurWeapon->GetAnimState())
	{
		CurWeapon->ArrowReady = true;
	}
}

void UMainCharAnimInstance::AnimNotify_ChordToHand()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(character->BowWeaponMesh->GetAnimInstance());

	BowAnim->SetBowChordCheck(true);
}

void UMainCharAnimInstance::AnimNotify_ReRoad()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->GetCurWeaponAction()->ArrowSpawn();
}

void UMainCharAnimInstance::AnimNotify_StartAttack()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	EWeaponType WeaponT = CurWeapon->GetWeaponType();

	if (EWeaponType::Bow == WeaponT)
	{
		UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(character->BowWeaponMesh->GetAnimInstance());

		if (nullptr != BowAnim)
		{
			BowAnim->SetBowChordCheck(false);
		}

		AArrow* CurArrow = CurWeapon->GetReadyArrow();

		if (nullptr != CurArrow)
		{
			CurArrow->SetIsLocationAndRotation(false);

			bool IsAim = CurWeapon->GetIsAimOn();

			if (nullptr == character->GetController())
			{
				return;
			}

			FRotator ArrowRotationValue = character->GetControlRotation();

			AController* CharController = character->GetController();

			if (nullptr == CharController)
			{
				return;
			}

			// 발사 방향
			if (false == IsAim)
			{
				FVector CharForwardVector = character->GetActorForwardVector();

				CurArrow->FireInDirection(CharForwardVector, ArrowRotationValue, CharController);
			}
			else if (true == IsAim && nullptr != character->GetController())
			{
				AMainCharacter* MainChar = Cast<AMainCharacter>(character);
				FVector TraceTarget = MainChar->CameraLineTrace();

				CurArrow->FireInDirection(TraceTarget, ArrowRotationValue, CharController);
			}

			// 콜리전 변경
			CurWeapon->ChangeCollisionAttackType();

			CurWeapon->SetnullReadyArrow();
		}
	}
	else if (EWeaponType::Sword == WeaponT || EWeaponType::UnArmed == WeaponT)
	{
		CurWeapon->ChangeCollisionAttackType();
	}
}

void UMainCharAnimInstance::AnimNotify_EndAttack()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	CurWeapon->ChangeNoCollision();
}

void UMainCharAnimInstance::AnimNotify_Death()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->Destroy();
}

void UMainCharAnimInstance::AnimNotify_ParryOnOff()
{
	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	bool ParryState = CurWeapon->GetIsParry();

	if (false == ParryState)
	{
		CurWeapon->SetIsParry(true);
	}
	else
	{
		CurWeapon->SetIsParry(false);
	}
}

void UMainCharAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UMainCharAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UMainCharAnimInstance::MontageBlendingOut);

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	struct FAnimaitionData* FindAnimaitionData = Instance->GetAllAnimaitionDatas(TEXT("UnArmed"));

	if (nullptr == FindAnimaitionData)
	{
		return;
	}

	SetAllAnimation<CharacterAnimState>(FindAnimaitionData->AllAnimations);
}

void UMainCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (0 == AllAnimations.Num())
	{
		return;
	}

	AWeaponCharacter* character = Cast<AWeaponCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* CurWeapon = character->GetCurWeaponAction();

	if (nullptr == CurWeapon)
	{
		return;
	}

	if (false == AllAnimations.Contains(CurWeapon->GetAnimStateToInt()))
	{
		return;
	}

	UAnimMontage* Montage = AllAnimations[CurWeapon->GetAnimStateToInt()];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		// 무기가 있는 상태에서 없는 상태로 갈 땐 역재생
		if (EWeaponType::UnArmed == CurWeapon->GetWeaponType() && (CharacterAnimState::EquipOrDisArmBow == CurWeapon->GetAnimState() || CharacterAnimState::EquipOrDisArmSwordAndShield == CurWeapon->GetAnimState()))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, CurWeapon->GetAnimStateToInt());
			Montage_Play(Montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// 나머지 일반적인 애니메이션 재생
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, CurWeapon->GetAnimStateToInt());
			Montage_Play(Montage, AnimSpeed);
		}
	}
}
