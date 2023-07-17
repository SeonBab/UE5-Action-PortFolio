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
	// 걸으면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::Run:
		// 걷는 입력이 없다면 달리기가 중지되게 해준다
		if (0.f == _Value)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// 걷는다.
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
	// 걸으면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::Run:
		// 걷는 입력이 없다면 달리기가 중지되게 해준다
		if (0.f == _Value)
		{
			AnimState = MainCharacterAnimState::Idle;
		}
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	// 걷는다.
	if (nullptr != CurCharacter->Controller && _Value != 0.0f)
	{
		const FRotator Rotation = CurCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		AnimState = MainCharacterAnimState::Walk;
		CurCharacter->AddMovementInput(Direction, _Value);
	}
	// WAndSButtonAction()보다 늦게 호출되어 AnimState = MainCharacterAnimState::Idle을 안해줘도 다음 tick에 Idle로 변환됨
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
			// 구르면 안되는 상태
			switch (AnimState)
			{
			case MainCharacterAnimState::WalkJump:
			case MainCharacterAnimState::RunJump:
			case MainCharacterAnimState::Roll:
				return;
				break;
			}
			// 구른다
			if (nullptr != CurCharacter->Controller)
			{
				AnimState = MainCharacterAnimState::Roll;
			}
		}

		PressTime = 0;
		return;
	}

	++PressTime;

	// 달리면 안되는 상태
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
		// 달린다
		// 달리는 애니메이션이 계속 재생된다??
		// 좌우 움직임에서 입력값이 없으니 포워드에서 Idle로 바꾸게 만든다.
		AnimState = MainCharacterAnimState::Run;
	}
}

void UWeaponAction::ShiftButtonAction()
{
	// 점프하면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
	case MainCharacterAnimState::RunJump:
	case MainCharacterAnimState::Roll:
		return;
		break;
	}

	//점프한다
	AnimState = MainCharacterAnimState::WalkJump;
	CurCharacter->Jump();
}
