// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon/WeaponAction.h"
#include "Components/CapsuleComponent.h"

UWeaponAction::UWeaponAction()
{
	//CurCharacter = CreateDefaultSubobject<ACharacter>(TEXT("Weapon"));
	
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
	UE_LOG(LogTemp, Log, TEXT("AAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAAA"));
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
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// �ȴ´�.
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		AnimState = MainCharacterAnimState::Walk;
		CurCharacter->AddMovementInput(Direction, _Value);
	}
	else
	{
		if (AnimState == MainCharacterAnimState::Walk)
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
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// �ȴ´�.
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AnimState = MainCharacterAnimState::Walk;
		CurCharacter->AddMovementInput(Direction, _Value);
	}
	// WAndSButtonAction()���� �ʰ� ȣ��Ǿ� AnimState = MainCharacterAnimState::Idle�� �����൵ ���� tick�� Idle�� ��ȯ��
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
		// �޸��� �ִϸ��̼��� ��� ����ȴ�??
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
