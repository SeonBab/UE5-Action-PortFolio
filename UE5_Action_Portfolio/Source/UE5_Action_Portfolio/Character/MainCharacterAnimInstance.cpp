#include "Character/MainCharacterAnimInstance.h"
#include "MainCharacter.h"
#include "Components/SkeletalMeshComponent.h"
#include "Global/GlobalGameInstance.h"
#include "Global/AnimaitionData.h"
#include "Weapon/WeaponAction.h"

void UMainCharacterAnimInstance::MontageEnd(UAnimMontage* Anim, bool Inter)
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	if (AllAnimations[CharacterAnimState::WalkJump] == Anim || AllAnimations[CharacterAnimState::RunJump] == Anim || 
		AllAnimations[CharacterAnimState::Roll] == Anim || AllAnimations[CharacterAnimState::Attack] == Anim)
	{
		Animstate = CharacterAnimState::Idle;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[CharacterAnimState::Idle], 1.0f);
		character->CurWeaponAction->SetCharacterAirControl(1.f);
	}
	else if (AllAnimations[CharacterAnimState::EquipOrDisArmBow] == Anim || AllAnimations[CharacterAnimState::EquipOrDisArmSwordAndShield] == Anim)
	{
		Animstate = CharacterAnimState::Idle;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[CharacterAnimState::Idle], 1.0f);
		AnimSpeed = 1.f;

		if (true == character->CurWeaponAction->SwordAndSheiledToBow)
		{
			character->ChangeBow();
			character->CurWeaponAction->SwordAndSheiledToBow = false;
		}
		else if (true == character->CurWeaponAction->BowToSwordAndSheiled)
		{
			character->ChangeSwordAndSheiled();
			character->CurWeaponAction->BowToSwordAndSheiled = false;
		}
	}
}

void UMainCharacterAnimInstance::AnimNotify_RollStop()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->CurWeaponAction->IsRollMoveToFalse();
}

void UMainCharacterAnimInstance::AnimNotify_JumpStart()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	character->Jump();
}

void UMainCharacterAnimInstance::AnimNotify_ChangeWeapon()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

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

void UMainCharacterAnimInstance::AnimNotify_AttackCheck()
{
	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	bool Value = character->CurWeaponAction->AttackCheck;

	if (0 == Value)
	{
		Animstate = CharacterAnimState::Idle;
		character->CurWeaponAction->SetAnimState(Animstate);
		Montage_Play(AllAnimations[CharacterAnimState::Idle], 1.0f);
	}
	else
	{
		character->CurWeaponAction->AttackCheck = false;
	}
}

void UMainCharacterAnimInstance::NativeInitializeAnimation()
{
}

void UMainCharacterAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OnMontageBlendingOut.AddDynamic(this, &UMainCharacterAnimInstance::MontageEnd);

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

void UMainCharacterAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	if (0 == AllAnimations.Num())
	{
		return;
	}

	AMainCharacter* character = Cast<AMainCharacter>(GetOwningActor());

	if (nullptr == character && false == character->IsValidLowLevel())
	{
		return;
	}

	Animstate = *character->AnimState;

	if (false == AllAnimations.Contains(Animstate))
	{
		return;
	}

	UAnimMontage* montage = AllAnimations[Animstate];

	if (nullptr == montage)
	{
		return;
	}

	if (false == Montage_IsPlaying(montage))
	{
		// ���Ⱑ �ִ� ���¿��� ���� ���·� �� �� �����
		if (EWeaponType::UnArmed == character->CurWeaponAction->WeaponType && (CharacterAnimState::EquipOrDisArmBow == Animstate || CharacterAnimState::EquipOrDisArmSwordAndShield == Animstate))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, static_cast<int>(Animstate));
			Montage_Play(montage, AnimSpeed, EMontagePlayReturnType::MontageLength, 1.f);
		}
		// ������ �Ϲ����� �ִϸ��̼� ���
		else
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> %d"), __FUNCTION__, __LINE__, static_cast<int>(Animstate));
			Montage_Play(montage, AnimSpeed);
		}

	}
}
