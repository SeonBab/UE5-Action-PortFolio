// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponAction.h"
#include "Global/GlobalGameInstance.h"
#include "Global/WeaponData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Character/MainCharacterAnimInstance.h"

UWeaponAction::UWeaponAction()
{
	//CurCharacter = CreateDefaultSubobject<ACharacter>(TEXT("Weapon"));
	
	// 틱 사용 어떻게 하지?
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
	// SpaceBar 누른 시간 체크
	if (true == PressSpacebar)
	{
		PressSpacebarTime += _DeltaTime;
	}

	// 굴렀을 때 움직이기
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
	// 이동하면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::Roll:
	case MainCharacterAnimState::WalkJump:
		return;
		break;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// 걷는다
		if (AnimState == MainCharacterAnimState::Idle || AnimState == MainCharacterAnimState::Walk)
		{
			IsForwardWalk = true;

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;

			AnimState = MainCharacterAnimState::Walk;
		}
		// 달린다
		else if (AnimState == MainCharacterAnimState::Run)
		{

		}

		// 점프 하며 공중에 있는게 아니라면 이동 불가능
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
		// 앞뒤 입력이 없으면 false
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
	// 이동하면 안되는 상태
	switch (AnimState)
	{
	case MainCharacterAnimState::Roll:
	case MainCharacterAnimState::WalkJump:
		return;
		break;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// 걷는다
		if (AnimState == MainCharacterAnimState::Idle || AnimState == MainCharacterAnimState::Walk)
		{
			IsLeftWalk = true;

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = 600;

			AnimState = MainCharacterAnimState::Walk;
		}
		// 달린다
		else if (AnimState == MainCharacterAnimState::Run)
		{

		}

		// 점프 하며 공중에 있는게 아니라면 이동 불가능
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
		// 좌우 입력이 없으면 false
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

		// 짧게 입력이 들어왔는지 확인
		if (nullptr != CurCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
		{
			// 구르면 안되는 상태
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
			// 구른다
			if (nullptr != CurCharacter->Controller)
			{
				IsRollMove = true;
				AnimState = MainCharacterAnimState::Roll;
			}
		}

		// 입력이 멈추면 누른 시간 0
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	}

	// 누른 시간 체크 시작
	PressSpacebar = true;
	PressSpacebarTime++;

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

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// 달린다
		// 달리는 애니메이션이 빨리 재생된다???
		AnimState = MainCharacterAnimState::Run;

		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = 900;
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

	// 달릴 때 점프
	if (MainCharacterAnimState::Run == AnimState)
	{
		AnimState = MainCharacterAnimState::RunJump;
		CurCharacter->Jump();
	}
	// 걸을 때, 가만히 있을 때 점프
	else if (MainCharacterAnimState::Walk == AnimState || MainCharacterAnimState::Idle == AnimState)
	{
		AnimState = MainCharacterAnimState::WalkJump;
	}

	CurCharacter->GetCharacterMovement()->AirControl = 0.3f;
}