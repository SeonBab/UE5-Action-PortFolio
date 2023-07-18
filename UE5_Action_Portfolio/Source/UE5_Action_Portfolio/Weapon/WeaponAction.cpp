// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponAction.h"
#include "Global/GlobalGameInstance.h"
#include "Global/WeaponData.h"
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
	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// �̵��Ѵ�
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// �ȴ´�
		if (AnimState == MainCharacterAnimState::Idle || AnimState == MainCharacterAnimState::Walk)
		{
			IsForwardWalk = true;

			AnimState = MainCharacterAnimState::Walk;
		}
		// �޸���
		else if (AnimState == MainCharacterAnimState::Run)
		{

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

		if (AnimState == MainCharacterAnimState::Walk && false == IsLeftWalk)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	}
}

void UWeaponAction::DAndAButtonAction(float _Value)
{
	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// �̵��Ѵ�
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// �ȴ´�
		if (AnimState == MainCharacterAnimState::Idle || AnimState == MainCharacterAnimState::Walk)
		{
			IsLeftWalk = true;

			AnimState = MainCharacterAnimState::Walk;
		}
		// �޸���
		else if (AnimState == MainCharacterAnimState::Run)
		{

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
		// �ȴ� �Է��� ������ �޸��� �ʱ�ȭ, ����
		if (false == IsForwardWalk && false == IsLeftWalk)
		{
			PressTime = 0;
			//AnimState = MainCharacterAnimState::Idle;
		}

		// ª�� �Է��� ���Դ��� Ȯ��
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

		//PressTime = 0;
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

	if (nullptr != CurCharacter->Controller && PressTime >= RunCount)
	{
		// �޸���
		// �޸��� �ִϸ��̼��� ���� ����ȴ�???
		AnimState = MainCharacterAnimState::Run;

		//const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		//const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		//CurCharacter->AddMovementInput(Direction, _Value);
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
