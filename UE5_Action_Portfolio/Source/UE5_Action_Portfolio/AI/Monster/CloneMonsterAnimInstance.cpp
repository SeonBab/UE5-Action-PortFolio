#include "AI/Monster/CloneMonsterAnimInstance.h"
#include "AI/Monster/CloneMonster.h"
#include "Global/Data/AnimaitionData.h"
#include "Global/GlobalGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/WeaponComponent.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"

void UCloneMonsterAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(Character->GetAnimState());

	if (CharacterAnimState::Death == AnimState)
	{
		return;
	}
	else if (CharacterAnimState::Dizzy == AnimState && GetAnimMontage(CharacterAnimState::Dizzy) != Anim)
	{
		return;
	}


	if (GetAnimMontage(CharacterAnimState::WalkJump) == Anim || GetAnimMontage(CharacterAnimState::RunJump) == Anim ||
		GetAnimMontage(CharacterAnimState::Roll) == Anim || GetAnimMontage(CharacterAnimState::Attack) == Anim ||
		GetAnimMontage(CharacterAnimState::GotHit) == Anim || GetAnimMontage(CharacterAnimState::Dizzy) == Anim)
	{
		if (false == WeaponComponent->GetIsLockOn())
		{
			Character->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == WeaponComponent->GetIsLockOn())
		{
			Character->SetAnimState(CharacterAnimState::LockOnIdle);
		}
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
		WeaponComponent->SetCharacterAirControl(1.f);
	}
	else if (GetAnimMontage(CharacterAnimState::EquipOrDisArmBow) == Anim || GetAnimMontage(CharacterAnimState::EquipOrDisArmSwordAndShield) == Anim)
	{
		if (false == WeaponComponent->GetIsLockOn())
		{
			Character->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == WeaponComponent->GetIsLockOn())
		{
			Character->SetAnimState(CharacterAnimState::LockOnIdle);
		}
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
		AnimSpeed = 1.f;

		if (true == WeaponComponent->SwordAndSheiledToBow)
		{
			WeaponComponent->ChangeSetBow();
			WeaponComponent->SwordAndSheiledToBow = false;
		}
		else if (true == WeaponComponent->BowToSwordAndSheiled)
		{
			WeaponComponent->ChangeSetSwordAndSheiled();
			WeaponComponent->BowToSwordAndSheiled = false;
		}
	}
	else if (GetAnimMontage(CharacterAnimState::ParryorFire) == Anim)
	{
		if (EWeaponType::Bow == WeaponComponent->GetWeaponType())
		{
			Character->SetAnimState(CharacterAnimState::Idle);
			Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
			WeaponComponent->ArrowReady = false;
			WeaponComponent->EarlyArrowCheck = false;
		}
		else if (EWeaponType::Sword == WeaponComponent->GetWeaponType())
		{
			if (false == WeaponComponent->GetIsLockOn())
			{
				Character->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == WeaponComponent->GetIsLockOn())
			{
				Character->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
			WeaponComponent->SetCharacterAirControl(1.f);
		}
	}
	else if (GetAnimMontage(CharacterAnimState::AimOrBlockGotHit) == Anim)
	{
		if (EWeaponType::Sword == WeaponComponent->GetWeaponType())
		{
			Character->SetAnimState(CharacterAnimState::AimOrBlock);

			// �ٽ� ���
			FName SectionName = "AimorBlock2";

			UAnimMontage* Montage = AllAnimations[Character->GetAnimState()];

			if (nullptr == Montage)
			{
				return;
			}

			Montage_JumpToSection(SectionName, Montage);
		}
		else if (false == WeaponComponent->IsLockOn)
		{
			Character->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == WeaponComponent->IsLockOn)
		{
			Character->SetAnimState(CharacterAnimState::LockOnIdle);
		}
	}
}

void UCloneMonsterAnimInstance::AnimNotify_RollStop()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	WeaponComponent->IsRollMoveToFalse();
}

void UCloneMonsterAnimInstance::AnimNotify_JumpStart()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	Character->Jump();
}

void UCloneMonsterAnimInstance::AnimNotify_ChangeWeapon()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();
	CharacterAnimState CurAnimState = static_cast<CharacterAnimState>(Character->GetAnimState());

	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Instance)
	{
		return;
	}

	// ��ȯ �Ϸ��� ���Ⱑ �Ǽ��� ��
	if (EWeaponType::UnArmed == CurWeaponType)
	{
		// Ȱ�� ��� ���� ��
		if (CharacterAnimState::EquipOrDisArmBow == CurAnimState)
		{
			WeaponComponent->BowWeaponMesh->SetSkeletalMesh(nullptr);
			WeaponComponent->BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
			WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}
		// Į�� ���и� ��� ���� ��
		else if (CharacterAnimState::EquipOrDisArmSwordAndShield == CurAnimState)
		{
			WeaponComponent->SwordWeaponMesh->SetSkeletalMesh(nullptr);
			WeaponComponent->ShieldWeaponMesh->SetSkeletalMesh(nullptr);
			WeaponComponent->BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
			WeaponComponent->BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
			WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}

	}
	// ��ȯ �Ϸ��� ���Ⱑ Ȱ�� ��
	else if (EWeaponType::Bow == CurWeaponType)
	{
		WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		WeaponComponent->BackBowWeaponMesh->SetSkeletalMesh(nullptr);
		WeaponComponent->BowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));

	}
	// ��ȯ �Ϸ��� ���Ⱑ Į�� ������ ��
	else if (EWeaponType::Sword == CurWeaponType)
	{
		WeaponComponent->SwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
		WeaponComponent->ShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
		WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		WeaponComponent->BackSwordWeaponMesh->SetSkeletalMesh(nullptr);
		WeaponComponent->BackShieldWeaponMesh->SetSkeletalMesh(nullptr);
	}
}

void UCloneMonsterAnimInstance::AnimNotify_AttackCheck()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(Character->GetAnimState());

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	bool Value = WeaponComponent->GetAttackCheck();

	if (CharacterAnimState::Attack == AnimState)
	{
		if (false == Value)
		{
			if (false == WeaponComponent->GetIsLockOn())
			{
				Character->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == WeaponComponent->GetIsLockOn())
			{
				Character->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			Montage_Play(AllAnimations[Character->GetAnimState()], 1.0f);
		}
		else
		{
			WeaponComponent->SetAttackCheck(false);
		}
	}
}

void UCloneMonsterAnimInstance::AnimNotify_AimorBlockCheck()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(Character->GetAnimState());

	if (CharacterAnimState::AimOrBlock == AnimState)
	{
		// �ٽ� ����or ���
		FName SectionName = "AimorBlock2";

		UAnimMontage* Montage = AllAnimations[Character->GetAnimState()];

		if (nullptr == Montage)
		{
			return;
		}

		Montage_JumpToSection(SectionName, Montage);
	}
}

void UCloneMonsterAnimInstance::AnimNotify_ArrowReadyCheck()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(Character->GetAnimState());

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	// �߻簡 �غ�Ǳ� ���� �̸� ���� �Է��� ������ ��
	if (true == WeaponComponent->EarlyArrowCheck && CharacterAnimState::AimOrBlock == AnimState)
	{
		WeaponComponent->ArrowReady = false;
		WeaponComponent->EarlyArrowCheck = false;

		Character->SetAnimState(CharacterAnimState::ParryorFire);
		Montage_Play(AllAnimations[Character->GetAnimState()], 1.f);
	}
	// �߻簡 �غ�� �� �Է��� Ȯ���� ��
	else if (false == WeaponComponent->EarlyArrowCheck && CharacterAnimState::AimOrBlock == AnimState)
	{
		WeaponComponent->ArrowReady = true;
	}
}

void UCloneMonsterAnimInstance::AnimNotify_ChordToHand()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(WeaponComponent->BowWeaponMesh->GetAnimInstance());

	if (false == IsValid(BowAnim))
	{
		return;
	}

	BowAnim->SetBowChordCheck(true);
}

void UCloneMonsterAnimInstance::AnimNotify_ReRoad()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	WeaponComponent->ArrowSpawn();
}

void UCloneMonsterAnimInstance::AnimNotify_StartAttack()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	EWeaponType WeaponT = WeaponComponent->GetWeaponType();

	if (EWeaponType::Bow == WeaponT)
	{
		UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(WeaponComponent->BowWeaponMesh->GetAnimInstance());

		if (nullptr != BowAnim)
		{
			BowAnim->SetBowChordCheck(false);
		}

		AArrow* CurArrow = WeaponComponent->GetReadyArrow();

		if (nullptr != CurArrow)
		{
			CurArrow->SetIsLocationAndRotation(false);

			bool IsAim = WeaponComponent->GetIsAimOn();

			if (nullptr == Character->GetController())
			{
				return;
			}

			FRotator ArrowRotationValue = Character->GetControlRotation();

			AController* CharController = Character->GetController();

			if (nullptr == CharController)
			{
				return;
			}

			// �߻� ����
			if (false == IsAim)
			{
				FVector CharForwardVector = Character->GetActorForwardVector();

				CurArrow->FireInDirection(CharForwardVector, ArrowRotationValue, CharController);
			}
			else if (true == IsAim && nullptr != Character->GetController())
			{
				// Ŭ�� ���ʹ� ���� ����� ���� �ʴ´�
			}

			// �ݸ��� ����
			WeaponComponent->ChangeCollisionAttackType();

			WeaponComponent->SetnullReadyArrow();
		}
	}
	else if (EWeaponType::Sword == WeaponT || EWeaponType::UnArmed == WeaponT)
	{
		WeaponComponent->ChangeCollisionAttackType();
	}
}

void UCloneMonsterAnimInstance::AnimNotify_EndAttack()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (nullptr == Character && false == Character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	WeaponComponent->OverlapEnd();
}

void UCloneMonsterAnimInstance::AnimNotify_Death()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	Character->Destroy();
}

void UCloneMonsterAnimInstance::AnimNotify_ParryOnOff()
{
	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	bool ParryState = WeaponComponent->GetIsParry();

	if (false == ParryState)
	{
		WeaponComponent->SetIsParry(true);
	}
	else
	{
		WeaponComponent->SetIsParry(false);
	}
}

void UCloneMonsterAnimInstance::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
}

void UCloneMonsterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UCloneMonsterAnimInstance::MontageBlendingOut);

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

void UCloneMonsterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	if (0 == AllAnimations.Num())
	{
		return;
	}

	ACloneMonster* Character = Cast<ACloneMonster>(GetOwningActor());

	if (false == IsValid(Character))
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(Character->GetAnimState());

	UWeaponComponent* WeaponComponent = Character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	if (false == AllAnimations.Contains(Character->GetAnimState()))
	{
		return;
	}

	UAnimMontage* Montage = AllAnimations[Character->GetAnimState()];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		// ���Ⱑ �ִ� ���¿��� ���� ���·� �� �� �����
		if (EWeaponType::UnArmed == WeaponComponent->GetWeaponType() && (CharacterAnimState::EquipOrDisArmBow == AnimState || CharacterAnimState::EquipOrDisArmSwordAndShield == AnimState))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, Character->GetAnimState());
			Montage_Play(Montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// ������ �Ϲ����� �ִϸ��̼� ���
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, Character->GetAnimState());
			Montage_Play(Montage, AnimSpeed);
		}
	}
}