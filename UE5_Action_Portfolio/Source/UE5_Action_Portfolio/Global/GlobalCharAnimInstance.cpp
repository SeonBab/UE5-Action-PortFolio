#include "Global/Data/AnimaitionData.h"
#include "Global/GlobalCharAnimInstance.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapon/WeaponAction.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"

void UGlobalCharAnimInstance::MontageBlendingOut(UAnimMontage* Anim, bool Inter)
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	if (AllAnimations[CharacterAnimState::WalkJump] == Anim || AllAnimations[CharacterAnimState::RunJump] == Anim || 
		AllAnimations[CharacterAnimState::Roll] == Anim || AllAnimations[CharacterAnimState::Attack] == Anim)
	{
		if (false == character->CurWeaponAction->IsLockOn)
		{
			Animstate = CharacterAnimState::Idle;
		}
		else if (true == character->CurWeaponAction->IsLockOn)
		{
			Animstate = CharacterAnimState::LockOnIdle;
		}
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[Animstate], 1.f);
		character->CurWeaponAction->SetCharacterAirControl(1.f);
	}
	else if (AllAnimations[CharacterAnimState::EquipOrDisArmBow] == Anim || AllAnimations[CharacterAnimState::EquipOrDisArmSwordAndShield] == Anim)
	{
		if (false == character->CurWeaponAction->IsLockOn)
		{
			Animstate = CharacterAnimState::Idle;
		}
		else if (true == character->CurWeaponAction->IsLockOn)
		{
			Animstate = CharacterAnimState::LockOnIdle;
		}
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[Animstate], 1.f);
		AnimSpeed = 1.f;

		if (true == character->CurWeaponAction->SwordAndSheiledToBow)
		{
			character->CurWeaponAction->ChangeSetBow();
			character->CurWeaponAction->SwordAndSheiledToBow = false;
		}
		else if (true == character->CurWeaponAction->BowToSwordAndSheiled)
		{
			character->CurWeaponAction->ChangeSetSwordAndSheiled();
			character->CurWeaponAction->BowToSwordAndSheiled = false;
		}
	}
	else if (AllAnimations[CharacterAnimState::ParryorFire] == Anim)
	{
		if (EWeaponType::Bow == character->CurWeaponAction->WeaponType)
		{
			Animstate = CharacterAnimState::Idle;
			character->CurWeaponAction->SetAnimState(Animstate);
			Montage_Play(AllAnimations[Animstate], 1.f);
			character->CurWeaponAction->ArrowReady = false;
			character->CurWeaponAction->EarlyArrowCheck = false;
		}
	}
}

void UGlobalCharAnimInstance::AnimNotify_RollStop()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->CurWeaponAction->IsRollMoveToFalse();
}

void UGlobalCharAnimInstance::AnimNotify_JumpStart()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->Jump();
}

void UGlobalCharAnimInstance::AnimNotify_ChangeWeapon()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}
	
	EWeaponType CurWeaponType = character->CurWeaponAction->WeaponType;
	CharacterAnimState CurAnimState = character->CurWeaponAction->AnimState;

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
			character->BowWeaponMesh->SetSkeletalMesh(nullptr);
			character->BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
			character->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}
		// Į�� ���и� ��� ���� ��
		else if (CharacterAnimState::EquipOrDisArmSwordAndShield == CurAnimState)
		{
			character->SwordWeaponMesh->SetSkeletalMesh(nullptr);
			character->ShieldWeaponMesh->SetSkeletalMesh(nullptr);
			character->BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
			character->BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
			character->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}

	}
	// ��ȯ �Ϸ��� ���Ⱑ Ȱ�� ��
	else if (EWeaponType::Bow == CurWeaponType)
	{
		character->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackBowWeaponMesh->SetSkeletalMesh(nullptr);
		character->BowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));

	}
	// ��ȯ �Ϸ��� ���Ⱑ Į�� ������ ��
	else if (EWeaponType::Sword == CurWeaponType)
	{
		character->SwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
		character->ShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
		character->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackSwordWeaponMesh->SetSkeletalMesh(nullptr);
		character->BackShieldWeaponMesh->SetSkeletalMesh(nullptr);
	}
}

void UGlobalCharAnimInstance::AnimNotify_AttackCheck()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	bool Value = character->CurWeaponAction->AttackCheck;

	if (CharacterAnimState::Attack == character->CurWeaponAction->AnimState)
	{
		if (false == Value)
		{
			if (false == character->CurWeaponAction->IsLockOn)
			{
				Animstate = CharacterAnimState::Idle;
			}
			else if (true == character->CurWeaponAction->IsLockOn)
			{
				Animstate = CharacterAnimState::LockOnIdle;
			}
			character->CurWeaponAction->SetAnimState(Animstate);
			Montage_Play(AllAnimations[Animstate], 1.0f);
		}
		else
		{
			character->CurWeaponAction->AttackCheck = false;
		}
	}
}

void UGlobalCharAnimInstance::AnimNotify_AimorBlockCheck()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	if (CharacterAnimState::AimOrBlock == character->CurWeaponAction->AnimState)
	{
		// �ٽ� ����or ���
		FName SectionName = "AimorBlock2";

		UAnimMontage* Montage = AllAnimations[Animstate];

		if (nullptr == Montage)
		{
			return;
		}

		Montage_JumpToSection(SectionName, Montage);
	}
}

void UGlobalCharAnimInstance::AnimNotify_ArrowReadyCheck()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	// �̸� ���� �Է��� ������ ��
	if (true == character->CurWeaponAction->EarlyArrowCheck && CharacterAnimState::AimOrBlock == Animstate)
	{
		character->CurWeaponAction->ArrowReady = false;
		character->CurWeaponAction->EarlyArrowCheck = false;

		Animstate = CharacterAnimState::ParryorFire;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[Animstate], 1.f);
	}
	// �غ�� �� �Է��� ������ �� ��
	else if (false == character->CurWeaponAction->EarlyArrowCheck)
	{
		character->CurWeaponAction->ArrowReady = true;
	}
}

void UGlobalCharAnimInstance::AnimNotify_ChordToHand()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(character->BowWeaponMesh->GetAnimInstance());

	BowAnim->SetBowChordCheck(true);
}

void UGlobalCharAnimInstance::AnimNotify_ReRoad()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->GetCurWeaponAction()->ArrowSpawn();
}

void UGlobalCharAnimInstance::AnimNotify_StartAttack()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}
	
	UWeaponAction* WeaponAction = character->GetCurWeaponAction();

	if (nullptr == WeaponAction)
	{
		return;
	}

	EWeaponType WeaponT = WeaponAction->GetWeaponType();

	if (EWeaponType::Bow == WeaponT)
	{
		UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(character->BowWeaponMesh->GetAnimInstance());

		if (nullptr != BowAnim)
		{
			BowAnim->SetBowChordCheck(false);
		}

		AArrow* CurArrow = WeaponAction->GetReadyArrow();

		if (nullptr != CurArrow)
		{
			CurArrow->SetIsLocationAndRotation(false);

			bool IsAim = WeaponAction->GetIsAimOn();

			// �߻� ����
			if (false == IsAim)
			{
				FVector ControllVector = character->GetController()->GetControlRotation().Vector();

				CurArrow->FireInDirection(ControllVector);
			}
			else if (true == IsAim && nullptr != character->GetController())
			{
				FVector CharForwardVector = character->GetActorForwardVector();

				CurArrow->FireInDirection(CharForwardVector);
			}

			// �ݸ��� ����
			character->GetCurWeaponAction()->ChangeCollisionAttackType();

			character->GetCurWeaponAction()->SetnullReadyArrow();
		}
	}
	else if (EWeaponType::Sword == WeaponT || EWeaponType::UnArmed == WeaponT)
	{
		character->GetCurWeaponAction()->ChangeCollisionAttackType();
	}
}

void UGlobalCharAnimInstance::AnimNotify_EndAttack()
{
	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	UWeaponAction* WeaponAc = character->GetCurWeaponAction();

	if (nullptr == WeaponAc)
	{
		return;
	}

	WeaponAc->ChangeNoCollision();
}

void UGlobalCharAnimInstance::NativeInitializeAnimation()
{
}

void UGlobalCharAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UGlobalCharAnimInstance::MontageBlendingOut);

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

	AllAnimations = FindAnimaitionData->AllAnimations;
}

void UGlobalCharAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AGlobalCharacter* character = Cast<AGlobalCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	Animstate = *character->AnimState;

	if (false == AllAnimations.Contains(Animstate))
	{
		return;
	}

	UAnimMontage* Montage = AllAnimations[Animstate];

	if (nullptr == Montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(Montage))
	{
		// ���Ⱑ �ִ� ���¿��� ���� ���·� �� �� �����
		if (EWeaponType::UnArmed == character->CurWeaponAction->WeaponType && (CharacterAnimState::EquipOrDisArmBow == Animstate || CharacterAnimState::EquipOrDisArmSwordAndShield == Animstate))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, static_cast<int>(Animstate));
			Montage_Play(Montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// ������ �Ϲ����� �ִϸ��̼� ���
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, static_cast<int>(Animstate));
			Montage_Play(Montage, AnimSpeed);
		}
	}
}
