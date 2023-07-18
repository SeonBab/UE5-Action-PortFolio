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

	IsForwardWalk = false;
	IsLeftWalk = false;
	IsRollMove = false;
	IsWalkJump = false;
	IsRunJump = false;
	PressSpacebar = false;
	PressSpacebarTime = 0;
	RunCount = 30;
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
}

void UWeaponAction::SetCharacterAirControl(float _Value)
{
	CurCharacter->GetCharacterMovement()->AirControl = _Value;
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

void UWeaponAction::PressSpaceBarCkeck(float _DeltaTime)
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

		DeltaLocation.X = 1000 * _DeltaTime;

		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
	}
}

void UWeaponAction::BeginPlay()
{
	
}

void UWeaponAction::WAndSButtonAction(float _Value)
{
	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case MainCharacterAnimState::Roll:
	case MainCharacterAnimState::WalkJump:
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

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;

			AnimState = MainCharacterAnimState::Walk;
		}
		// �޸���
		else if (AnimState == MainCharacterAnimState::Run)
		{

		}

		// ���� �ϸ� ���߿� �ִ°� �ƴ϶�� �̵� �Ұ���
		if (false == CurCharacter->GetCharacterMovement()->IsFalling() && (MainCharacterAnimState::WalkJump == AnimState || MainCharacterAnimState::RunJump == AnimState))
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
	case MainCharacterAnimState::Roll:
	case MainCharacterAnimState::WalkJump:
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

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;

			AnimState = MainCharacterAnimState::Walk;
		}
		// �޸���
		else if (AnimState == MainCharacterAnimState::Run)
		{

		}

		// ���� �ϸ� ���߿� �ִ°� �ƴ϶�� �̵� �Ұ���
		if (false == CurCharacter->GetCharacterMovement()->IsFalling() && MainCharacterAnimState::RunJump == AnimState)
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

		// ª�� �Է��� ���Դ��� Ȯ��
		if (nullptr != CurCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
		{
			// ������ �ȵǴ� ����
			switch (AnimState)
			{
			case MainCharacterAnimState::WalkJump:
			case MainCharacterAnimState::RunJump:
				return;
			case MainCharacterAnimState::Roll:
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}
			// ������
			if (nullptr != CurCharacter->Controller)
			{
				IsRollMove = true;
				AnimState = MainCharacterAnimState::Roll;
			}
		}

		// �Է��� ���߸� ���� �ð� 0
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	}

	// ���� �ð� üũ ����
	PressSpacebar = true;
	PressSpacebarTime++;

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

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// �޸���
		// �޸��� �ִϸ��̼��� ���� ����ȴ�???
		AnimState = MainCharacterAnimState::Run;

		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = 900;
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

	// �޸� �� ����
	if (MainCharacterAnimState::Run == AnimState)
	{
		AnimState = MainCharacterAnimState::RunJump;
		CurCharacter->Jump();
	}
	// ���� ��, ������ ���� �� ����
	else if (MainCharacterAnimState::Walk == AnimState || MainCharacterAnimState::Idle == AnimState)
	{
		AnimState = MainCharacterAnimState::WalkJump;
	}

	CurCharacter->GetCharacterMovement()->AirControl = 0.3f;
}