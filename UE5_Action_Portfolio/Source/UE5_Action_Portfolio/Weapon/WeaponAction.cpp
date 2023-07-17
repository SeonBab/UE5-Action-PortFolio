// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponAction.h"
#include "GameFramework/CharacterMovementComponent.h"

UWeaponAction::UWeaponAction()
{
	//CurCharacter = CreateDefaultSubobject<ACharacter>(TEXT("Weapon"));
	
	PressTime = 0;
	IsForwardWalk = false;
	IsLeftdWalk = false;
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
}

MainCharacterAnimState* UWeaponAction::GetAnimState()
{
	return &AnimState;
}

void UWeaponAction::SetAnimState(MainCharacterAnimState _State)
{
	AnimState = _State;
}

void UWeaponAction::BeginPlay()
{
	
}



void UWeaponAction::WAndSButtonAction(float _Value)
{
	// ������ �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::Run:
		// �ȴ� �Է��� ���ٸ� �޸��Ⱑ �����ǰ� ���ش�
		if (0.f == _Value && false == IsLeftdWalk)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// �ȴ´�.
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		IsForwardWalk = true;

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		AnimState = MainCharacterAnimState::Walk;
		CurCharacter->AddMovementInput(Direction, _Value);
	}
	else
	{
		IsForwardWalk = false;

		if (AnimState == MainCharacterAnimState::Walk && false == IsLeftdWalk)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	}
}

void UWeaponAction::DAndAButtonAction(float _Value)
{
	// ������ �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::Run:
		// �ȴ� �Է��� ���ٸ� �޸��Ⱑ �����ǰ� ���ش�
		if (0.f == _Value && false == IsForwardWalk)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// �ȴ´�.
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		IsLeftdWalk = true;

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AnimState = MainCharacterAnimState::Walk;
		CurCharacter->AddMovementInput(Direction, _Value);
	}
	else
	{
		IsLeftdWalk = false;

		if (AnimState == MainCharacterAnimState::Walk && false == IsForwardWalk)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	}
}

void UWeaponAction::RollorRunAction(float _Value)
{
	if (0.f == _Value)
	{
		if (AnimState == MainCharacterAnimState::Run)
		{
			AnimState = MainCharacterAnimState::Idle;
		}

		if (0 != PressTime && PressTime <= 30)
		{
			// ������ �ȵǴ� ����
			switch (AnimState)
			{
			case MainCharacterAnimState::WalkJump:
			case MainCharacterAnimState::RunJump:
			case MainCharacterAnimState::Roll:
				return;
				break;
			}
			// ������
			if (nullptr != CurCharacter->Controller)
			{
				AnimState = MainCharacterAnimState::Roll;
			}
		}

		PressTime = 0;
		return;
	}

	++PressTime;

	// �޸��� �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::Idle:
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	if (PressTime >= 30)
	{
		// �޸���
		// �޸��� �ִϸ��̼��� ���� ����ȴ�???
		AnimState = MainCharacterAnimState::Run;
	}
}

void UWeaponAction::ShiftButtonAction()
{
	// �����ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	//�����Ѵ�
	AnimState = MainCharacterAnimState::WalkJump;
	CurCharacter->Jump();
}
