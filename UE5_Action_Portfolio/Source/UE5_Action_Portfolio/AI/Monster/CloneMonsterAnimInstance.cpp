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
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

			// 다시 블락
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

void UCloneMonsterAnimInstance::AnimNotify_RollStop()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

void UCloneMonsterAnimInstance::AnimNotify_JumpStart()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->Jump();
}

void UCloneMonsterAnimInstance::AnimNotify_ChangeWeapon()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

	// 전환 하려는 무기가 맨손일 때
	if (EWeaponType::UnArmed == CurWeaponType)
	{
		// 활을 들고 있을 때
		if (CharacterAnimState::EquipOrDisArmBow == CurAnimState)
		{
			WeaponComponent->BowWeaponMesh->SetSkeletalMesh(nullptr);
			WeaponComponent->BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
			WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}
		// 칼과 방패를 들고 있을 때
		else if (CharacterAnimState::EquipOrDisArmSwordAndShield == CurAnimState)
		{
			WeaponComponent->SwordWeaponMesh->SetSkeletalMesh(nullptr);
			WeaponComponent->ShieldWeaponMesh->SetSkeletalMesh(nullptr);
			WeaponComponent->BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
			WeaponComponent->BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));
			WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));
		}

	}
	// 전환 하려는 무기가 활일 때
	else if (EWeaponType::Bow == CurWeaponType)
	{
		WeaponComponent->UnArmedWeaponMesh->SetSkeletalMesh(nullptr);
		WeaponComponent->BackBowWeaponMesh->SetSkeletalMesh(nullptr);
		WeaponComponent->BowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));

	}
	// 전환 하려는 무기가 칼과 방패일 때
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
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

void UCloneMonsterAnimInstance::AnimNotify_AimorBlockCheck()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	CharacterAnimState AnimState = static_cast<CharacterAnimState>(character->GetAnimState());

	if (CharacterAnimState::AimOrBlock == AnimState)
	{
		// 다시 에임or 블락
		FName SectionName = "AimorBlock2";

		UAnimMontage* Montage = AllAnimations[character->GetAnimState()];

		if (nullptr == Montage)
		{
			return;
		}

		Montage_JumpToSection(SectionName, Montage);
	}
}

void UCloneMonsterAnimInstance::AnimNotify_ArrowReadyCheck()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

	// 발사가 준비되기 전에 미리 공격 입력을 눌렀을 때
	if (true == WeaponComponent->EarlyArrowCheck && CharacterAnimState::AimOrBlock == AnimState)
	{
		WeaponComponent->ArrowReady = false;
		WeaponComponent->EarlyArrowCheck = false;

		character->SetAnimState(CharacterAnimState::ParryorFire);
		Montage_Play(AllAnimations[character->GetAnimState()], 1.f);
	}
	// 발사가 준비된 후 입력을 확인할 때
	else if (false == WeaponComponent->EarlyArrowCheck && CharacterAnimState::AimOrBlock == AnimState)
	{
		WeaponComponent->ArrowReady = true;
	}
}

void UCloneMonsterAnimInstance::AnimNotify_ChordToHand()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

void UCloneMonsterAnimInstance::AnimNotify_ReRoad()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

void UCloneMonsterAnimInstance::AnimNotify_StartAttack()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

			// 발사 방향
			if (false == IsAim)
			{
				FVector CharForwardVector = character->GetActorForwardVector();

				CurArrow->FireInDirection(CharForwardVector, ArrowRotationValue, CharController);
			}
			else if (true == IsAim && nullptr != character->GetController())
			{
				// 클론 몬스터는 조준 사격을 하지 않는다
			}

			// 콜리전 변경
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
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

void UCloneMonsterAnimInstance::AnimNotify_Death()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

	if (nullptr == character || false == character->IsValidLowLevel())
	{
		return;
	}

	character->Destroy();
}

void UCloneMonsterAnimInstance::AnimNotify_ParryOnOff()
{
	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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

	ACloneMonster* character = Cast<ACloneMonster>(GetOwningActor());

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
		// 무기가 있는 상태에서 없는 상태로 갈 땐 역재생
		if (EWeaponType::UnArmed == WeaponComponent->GetWeaponType() && (CharacterAnimState::EquipOrDisArmBow == AnimState || CharacterAnimState::EquipOrDisArmSwordAndShield == AnimState))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, character->GetAnimState());
			Montage_Play(Montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// 나머지 일반적인 애니메이션 재생
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, character->GetAnimState());
			Montage_Play(Montage, AnimSpeed);
		}
	}
}