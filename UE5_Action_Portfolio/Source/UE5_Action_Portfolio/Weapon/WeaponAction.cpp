// Fill out your copyright notice in the Description page of Project Settings.

#include "Global/GlobalGameInstance.h"
#include "Global/WeaponData.h"
#include "Weapon/WeaponAction.h"
#include "GameFramework/CharacterMovementComponent.h"

UWeaponAction::UWeaponAction()
{
	//CurCharacter = CreateDefaultSubobject<ACharacter>(TEXT("Weapon"));
	
	// 틱 사용 어떻게 하지?
	//PrimaryActorTick.bCanEverTick = true;

	PressTime = 0;
	IsForwardWalk = false;
	IsLeftdWalk = false;
	IsRollMove = false;

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
	UGlobalGameInstance* Ins = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Ins)
	{
		return;
	}

	FWeaponData* FindWeaponData = Ins->GetWeaponData(_Weapon);

	if (nullptr == FindWeaponData)
	{
		return;
	}

	AllAnimations = FindWeaponData->AllAnimations;
}

void UWeaponAction::BeginPlay()
{
	
}

//void UWeaponAction::Tick(float _DeltaTime)
//{
//	// 틱 안들어오는데 어떻게 사용하지?
//	
//	// 굴렀을 때 움직이기
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
	// 걸으면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
		return;
	case MainCharacterAnimState::Run:
		// 걷는 입력이 없다면 달리기가 중지되게 해준다
		if (0.f == _Value && false == IsLeftdWalk)
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
	// 걸으면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::WalkJump:
		return;
	case MainCharacterAnimState::Run:
		// 걷는 입력이 없다면 달리기가 중지되게 해준다
		if (0.f == _Value && false == IsForwardWalk)
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
				IsRollMove = true;

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
		// 달리는 애니메이션이 빨리 재생된다???
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
