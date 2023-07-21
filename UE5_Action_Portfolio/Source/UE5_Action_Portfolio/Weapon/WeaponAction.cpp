// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponAction.h"
#include "Global/GlobalGameInstance.h"
#include "Global/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MainCharacterAnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"

UWeaponAction::UWeaponAction()
{	

}

void UWeaponAction::BeginPlay()
{
}

void UWeaponAction::Tick(float _DeltaTime)
{
	PressSpaceBarCkeckAndRoll(_DeltaTime);
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
}

void UWeaponAction::SetCharacterAirControl(float _Value)
{
	CurCharacter->GetCharacterMovement()->AirControl = _Value;
}

CharacterAnimState* UWeaponAction::GetAnimState()
{
	return &AnimState;
}

void UWeaponAction::SetAnimState(CharacterAnimState _State)
{
	AnimState = _State;
}

void UWeaponAction::IsRollMoveToFalse()
{
	IsRollMove = false;
}

void UWeaponAction::ChangeWeapon(FName _Weapon)
{
	UMainCharacterAnimInstance* Ptr = Cast<UMainCharacterAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	UGlobalGameInstance* Instacne = CurCharacter->GetWorld()->GetGameInstance<UGlobalGameInstance>();

	const struct FAnimaitionData* FindAnimationData = Instacne->GetAllAnimaitionDatas(_Weapon);

	if (nullptr == FindAnimationData)
	{
		return;
	}

	Ptr->AllAnimations = FindAnimationData->AllAnimations;
	WeaponType = FindAnimationData->Type;
	int a = 0;
}

void UWeaponAction::ChangeSetUnArmed()
{
	// Idle�� ���� ���� ����, ��� ���� �־�� UnArmed ����
	if (CharacterAnimState::Idle != AnimState || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// ������� ���� AnimInstance
	UMainCharacterAnimInstance* Ptr = Cast<UMainCharacterAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	// �����ϰ� �ִ� ��� Ȱ�� ��
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmBow;
	}
	// �����ϰ� �ִ� ��� Į�� ��
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponAction::ChangeSetBow()
{
	// Idle�� ������ ���´� ����
	if (CharacterAnimState::Idle != AnimState)
	{
		return;
	}
	// ���� ����� ����� ��� ���� ����
	else if (EWeaponType::Bow == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// �ٸ� ����� ����� ��ü�ϰ� ���
	else if (EWeaponType::Sword == WeaponType)
	{
		// ��� ���� �ִϸ��̼��� �ȵ�
		ChangeSetUnArmed();
		SwordAndSheiledToBow = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmBow;

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponAction::ChangeSetSwordAndSheiled()
{
	// Idle�� ������ ���´� ����
	if (CharacterAnimState::Idle != AnimState)
	{
		return;
	}
	//���� ����� ����� ��� ���� ����
	else if (EWeaponType::Sword == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// �ٸ� ����� ����� ��ü�ϰ� ���
	else if (EWeaponType::Bow == WeaponType)
	{
		// ��� ���� �ִϸ��̼��� �ȵ�
		ChangeSetUnArmed();
		BowToSwordAndSheiled = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;

	ChangeWeapon(TEXT("SwordAndShield"));
}

void UWeaponAction::PressSpaceBarCkeckAndRoll(float _DeltaTime)
{
	// SpaceBar ���� �ð� üũ
	if (true == PressSpacebar)
	{
		PressSpacebarTime += _DeltaTime;
	}

	// ������ �� �����̱�
	if (true == IsRollMove)
	{
		FVector DeltaLocation = CurCharacter->GetActorRotation().Vector();

		DeltaLocation.X = 700 * _DeltaTime;

		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
	}
}

void UWeaponAction::AttackAction(float _Value)
{
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
		return;
		break;
	}
	
	if (0.f != _Value)
	{
		AnimState = CharacterAnimState::Attack;

	}

}

void UWeaponAction::WAndSButtonAction(float _Value)
{
	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
		return;
		break;
	}

	// �̵��Ѵ�
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// �ȴ´�
		if (AnimState == CharacterAnimState::Idle || AnimState == CharacterAnimState::Walk)
		{
			IsForwardWalk = true;

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			AnimState = CharacterAnimState::Walk;
		}
		// �޸���
		else if (AnimState == CharacterAnimState::Run)
		{
			IsForwardWalk = true;
		}

		// ���� �ϸ� ���߿� �ִ°� �ƴ϶�� �̵� �Ұ���
		if (false == CurCharacter->GetCharacterMovement()->IsFalling() && (CharacterAnimState::WalkJump == AnimState || CharacterAnimState::RunJump == AnimState))
		{
			return;
		}

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		CurCharacter->AddMovementInput(Direction, _Value);

	}
	else
	{
		// �յ� �Է��� ������ false
		if (0.f == _Value)
		{
			IsForwardWalk = false;
		}

		if (AnimState == CharacterAnimState::Walk && false == IsLeftWalk)
		{
			AnimState = CharacterAnimState::Idle;
		}
	}
}

void UWeaponAction::DAndAButtonAction(float _Value)
{
	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
		return;
		break;
	}

	// �̵��Ѵ�
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// �ȴ´�
		if (AnimState == CharacterAnimState::Idle || AnimState == CharacterAnimState::Walk)
		{
			IsLeftWalk = true;

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			AnimState = CharacterAnimState::Walk;
		}
		// �޸���
		else if (AnimState == CharacterAnimState::Run)
		{
			IsLeftWalk = true;
		}

		// ���� �ϸ� ���߿� �ִ°� �ƴ϶�� �̵� �Ұ���
		if (false == CurCharacter->GetCharacterMovement()->IsFalling() && CharacterAnimState::RunJump == AnimState)
		{
			return;
		}

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		CurCharacter->AddMovementInput(Direction, _Value);
	}
	else
	{
		// �¿� �Է��� ������ false
		if (0.f == _Value)
		{
			IsLeftWalk = false;
		}

		if (AnimState == CharacterAnimState::Walk && false == IsForwardWalk)
		{
			AnimState = CharacterAnimState::Idle;
		}
	}
}

void UWeaponAction::RollorRunAction(float _Value)
{
	if (0.f == _Value)
	{
		if (AnimState == CharacterAnimState::Run)
		{
			AnimState = CharacterAnimState::Idle;
		}

		// ª�� �Է��� ���Դ��� Ȯ��
		if (nullptr != CurCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
		{
			// ������ �ȵǴ� ����
			switch (AnimState)
			{
			case CharacterAnimState::WalkJump:
			case CharacterAnimState::RunJump:
			case CharacterAnimState::Roll:
			case CharacterAnimState::EquipOrDisArmBow:
			case CharacterAnimState::EquipOrDisArmSwordAndShield:
			case CharacterAnimState::Attack:
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}
			
			// ������
			IsRollMove = true;
			AnimState = CharacterAnimState::Roll;
		}

		// �Է��� ���߸� ���� �ð� 0
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	}

	// �޸��� �ȵǴ� ����
	if (false == IsForwardWalk && false == IsLeftWalk)
	{
		PressSpacebarTime = 0;
		PressSpacebar = false;
		AnimState = CharacterAnimState::Idle;
		return;
	}

	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
		break;
	}

	// ���� �ð� üũ ����
	if (false == PressSpacebar)
	{
		PressSpacebar = true;
	}

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// �޸���
		// �޸��� �ִϸ��̼��� ���� ����ȴ�???
		AnimState = CharacterAnimState::Run;

		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UWeaponAction::ShiftButtonAction()
{
	// �����ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
		return;
		break;
	}

	// �޸� �� ����
	if (CharacterAnimState::Run == AnimState)
	{
		AnimState = CharacterAnimState::RunJump;
		CurCharacter->Jump();
	}
	// ���� ��, ������ ���� �� ����
	else if (CharacterAnimState::Walk == AnimState || CharacterAnimState::Idle == AnimState)
	{
		AnimState = CharacterAnimState::WalkJump;
	}

	CurCharacter->GetCharacterMovement()->AirControl = 0.3f;
}