#include "Character/MainCharAnimInstance.h"
#include "Global/Data/AnimaitionData.h"
#include "Global/GlobalGameInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/WeaponComponent.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"
#include "Character/MainCharacter.h"

void UMainCharAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;

	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(character->GetAnimState());

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
			character->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == WeaponComponent->GetIsLockOn())
		{
			character->SetAnimState(CharacterAnimState::LockOnIdle);
		}
		Montage_Play(AllAnimations[character->GetAnimState()], 1.f);
		WeaponComponent->SetCharacterAirControl(1.f);
	}
	else if (GetAnimMontage(CharacterAnimState::EquipOrDisArmBow) == Anim || GetAnimMontage(CharacterAnimState::EquipOrDisArmSwordAndShield) == Anim)
	{
		if (false == WeaponComponent->GetIsLockOn())
		{
			character->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == WeaponComponent->GetIsLockOn())
		{
			character->SetAnimState(CharacterAnimState::LockOnIdle);
		}
		Montage_Play(AllAnimations[character->GetAnimState()], 1.f);
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
			character->SetAnimState(CharacterAnimState::Idle);
			Montage_Play(AllAnimations[character->GetAnimState()], 1.f);
			WeaponComponent->ArrowReady = false;
			WeaponComponent->EarlyArrowCheck = false;
		}
		else if (EWeaponType::Sword == WeaponComponent->GetWeaponType())
		{
			if (false == WeaponComponent->GetIsLockOn())
			{
				character->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == WeaponComponent->GetIsLockOn())
			{
				character->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			Montage_Play(AllAnimations[character->GetAnimState()], 1.f);
			WeaponComponent->SetCharacterAirControl(1.f);
		}
	}
	else if (GetAnimMontage(CharacterAnimState::AimOrBlockGotHit) == Anim)
	{
		if (EWeaponType::Sword == WeaponComponent->GetWeaponType())
		{
			character->SetAnimState(CharacterAnimState::AimOrBlock);

			// �ٽ� ���
			FName SectionName = "AimorBlock2";

			UAnimMontage* Montage = AllAnimations[character->GetAnimState()];

			if (nullptr == Montage)
			{
				return;
			}

			Montage_JumpToSection(SectionName, Montage);
		}
		else if (false == WeaponComponent->IsLockOn)
		{
			character->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == WeaponComponent->IsLockOn)
		{
			character->SetAnimState(CharacterAnimState::LockOnIdle);
		}
	}
}

void UMainCharAnimInstance::AnimNotify_RollStop()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	WeaponComponent->IsRollMoveToFalse();
}

void UMainCharAnimInstance::AnimNotify_JumpStart()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->Jump();
}

void UMainCharAnimInstance::AnimNotify_ChangeWeapon()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	EWeaponType CurWeaponType = WeaponComponent->GetWeaponType();
	CharacterAnimState CurAnimState = static_cast<CharacterAnimState>(character->GetAnimState());

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

void UMainCharAnimInstance::AnimNotify_AttackCheck()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(character->GetAnimState());

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

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
				character->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == WeaponComponent->GetIsLockOn())
			{
				character->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			Montage_Play(AllAnimations[character->GetAnimState()], 1.0f);
		}
		else
		{
			WeaponComponent->SetAttackCheck(false);
		}
	}
}

void UMainCharAnimInstance::AnimNotify_AimorBlockCheck()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(character->GetAnimState());

	if (CharacterAnimState::AimOrBlock == AnimState)
	{
		// �ٽ� ����or ���
		FName SectionName = "AimorBlock2";

		UAnimMontage* Montage = AllAnimations[character->GetAnimState()];

		if (nullptr == Montage)
		{
			return;
		}

		Montage_JumpToSection(SectionName, Montage);
	}
}

void UMainCharAnimInstance::AnimNotify_ArrowReadyCheck()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(character->GetAnimState());

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	// �߻簡 �غ�Ǳ� ���� �̸� ���� �Է��� ������ ��
	if (true == WeaponComponent->EarlyArrowCheck && CharacterAnimState::AimOrBlock == AnimState)
	{
		WeaponComponent->ArrowReady = false;
		WeaponComponent->EarlyArrowCheck = false;

		character->SetAnimState(CharacterAnimState::ParryorFire);
		Montage_Play(AllAnimations[character->GetAnimState()], 1.f);
	}
	// �߻簡 �غ�� �� �Է��� Ȯ���� ��
	else if (false == WeaponComponent->EarlyArrowCheck && CharacterAnimState::AimOrBlock == AnimState)
	{
		WeaponComponent->ArrowReady = true;
	}
}

void UMainCharAnimInstance::AnimNotify_ChordToHand()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(WeaponComponent->BowWeaponMesh->GetAnimInstance());

	if (nullptr == BowAnim || false == BowAnim->IsValidLowLevel())
	{
		return;
	}

	BowAnim->SetBowChordCheck(true);
}

void UMainCharAnimInstance::AnimNotify_ReRoad()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	WeaponComponent->ArrowSpawn();
}

void UMainCharAnimInstance::AnimNotify_StartAttack()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

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

			// �߻� ����
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

void UMainCharAnimInstance::AnimNotify_EndAttack()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	WeaponComponent->OverlapEnd();
}

void UMainCharAnimInstance::AnimNotify_Death()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->Destroy();
}

void UMainCharAnimInstance::AnimNotify_ParryOnOff()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

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

	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(character->GetAnimState());

	UWeaponComponent* WeaponComponent = character->GetWeaponComponent();

	if (nullptr == WeaponComponent || false == WeaponComponent->IsValidLowLevel())
	{
		return;
	}

	if (false == AllAnimations.Contains(character->GetAnimState()))
	{
		return;
	}

	UAnimMontage* Montage = AllAnimations[character->GetAnimState()];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		// ���Ⱑ �ִ� ���¿��� ���� ���·� �� �� �����
		if (EWeaponType::UnArmed == WeaponComponent->GetWeaponType() && (CharacterAnimState::EquipOrDisArmBow == AnimState || CharacterAnimState::EquipOrDisArmSwordAndShield == AnimState))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, character->GetAnimState());
			Montage_Play(Montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// ������ �Ϲ����� �ִϸ��̼� ���
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, character->GetAnimState());
			Montage_Play(Montage, AnimSpeed);
		}
	}
}
