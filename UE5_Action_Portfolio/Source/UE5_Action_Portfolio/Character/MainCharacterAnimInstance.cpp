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
