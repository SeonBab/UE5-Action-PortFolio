// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/GlobalGameInstance.h"
#include "Global/WeaponData.h"
#include "Weapon/WeaponAction.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MainCharacterAnimInstance.h"

UWeaponAction::UWeaponAction()
{
	//CurCharacter = CreateDefaultSubobject<ACharacter>(TEXT("Weapon"));
	
	// ƽ ��� ��� ����?
	//PrimaryActorTick.bCanEverTick = true;

	PressTime = 0;
	IsForwardWalk = false;
	IsLeftWalk = false;
	IsRollMove = false;
	RunCount = 50;
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

	const struct FWeaponData* FindWeaponData = Instacne->GetWeaponData(_Weapon);

	if (nullptr == FindWeaponData)
	{
		return;
	}

	Ptr->AllAnimations = FindWeaponData->AllAnimations;
}

void UWeaponAction::ChangeSetUnArmed()
{
	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponAction::ChangeSetBow()
{
	ChangeWeapon(TEXT("Bow"));
}

void UWeaponAction::ChangeSetSwordAndSheiled()
{
	ChangeWeapon(TEXT("SwordAndShield"));
}

void UWeaponAction::BeginPlay()
{
	
}

//void UWeaponAction::Tick(float _DeltaTime)
//{
//	// ƽ �ȵ����µ� ��� �������?
//	
//	// ������ �� �����̱�
//	if (true == IsRollMove)
//	{
//		FVector DeltaLocation = CurCharacter->GetActorRotation().Vector();
//
//		DeltaLocation.X = 1000 * CurCharacter->GetWorld()->GetDeltaSeconds();
//
//		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
//	}
//}

void UWeaponAction::WAndSButtonAction(float _Value)
{
	// ������ �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
		return;
	case MainCharacterAnimState::Run:
		// �ȴ� �Է��� ���ٸ� �޸��Ⱑ �����ǰ� ���ش�
		if (0.f == _Value && false == IsLeftWalk)
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

		if (AnimState == MainCharacterAnimState::Walk && false == IsLeftWalk)
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
		return;
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
		IsLeftWalk = true;

		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AnimState = MainCharacterAnimState::Walk;
		CurCharacter->AddMovementInput(Direction, _Value);
	}
	else
	{
		IsLeftWalk = false;

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

		if (0 != PressTime && PressTime <= RunCount)
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
				IsRollMove = true;

				AnimState = MainCharacterAnimState::Roll;
			}
		}

		PressTime = 0;
		return;
	}

	++PressTime;

	// �ȴ� �Է��� ������ �޸��� �ʱ�ȭ
	if (false == IsForwardWalk && false == IsLeftWalk)
	{
		PressTime = 0;
	}

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

	if (PressTime >= RunCount)
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
