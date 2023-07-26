// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponAction.h"
#include "Global/GlobalGameInstance.h"
#include "Global/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global/GlobalCharAnimInstance.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "Weapon/BowAnimInstance.h"

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
	UGlobalCharAnimInstance* Ptr = Cast<UGlobalCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

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
}

void UWeaponAction::ChangeSetUnArmed()
{
	// Idle만 무기 변경 가능, 장비를 끼고 있어야 UnArmed 가능
	if (CharacterAnimState::Idle != AnimState || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// 역재생을 위한 AnimInstance
	UGlobalCharAnimInstance* Ptr = Cast<UGlobalCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	// 착용하고 있던 장비가 활일 때
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmBow;
	}
	// 착용하고 있던 장비가 칼일 때
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponAction::ChangeSetBow()
{
	// Idle을 제외한 상태는 리턴
	if (CharacterAnimState::Idle != AnimState)
	{
		return;
	}
	// 같은 무기로 변경시 장비 장착 해제
	else if (EWeaponType::Bow == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// 다른 무기로 변경시 해체하고 장비
	else if (EWeaponType::Sword == WeaponType)
	{
		// 장비 변경 애니메이션이 안됨
		ChangeSetUnArmed();
		SwordAndSheiledToBow = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmBow;

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponAction::ChangeSetSwordAndSheiled()
{
	// Idle을 제외한 상태는 리턴
	if (CharacterAnimState::Idle != AnimState)
	{
		return;
	}
	//같은 무기로 변경시 장비 장착 해제
	else if (EWeaponType::Sword == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// 다른 무기로 변경시 해체하고 장비
	else if (EWeaponType::Bow == WeaponType)
	{
		// 장비 변경 애니메이션이 안됨
		ChangeSetUnArmed();
		BowToSwordAndSheiled = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;

	ChangeWeapon(TEXT("SwordAndShield"));
}

void UWeaponAction::PressSpaceBarCkeckAndRoll(float _DeltaTime)
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

		DeltaLocation.X = 700 * _DeltaTime;

		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
	}
}

void UWeaponAction::BowChordMove()
{
	if (EWeaponType::Bow == WeaponType && (CharacterAnimState::AimOrBlock != AnimState && CharacterAnimState::Attack != AnimState))
	{
		BowStringCheck = false;
	}

	//// GetAnimInstance가 null이다
	//if (true == BowStringCheck)
	//{
	//	USkeletalMeshComponent* SkeletalMeshComponent = CurCharacter->GetMesh();

	//	if (nullptr != SkeletalMeshComponent)
	//	{
	//		FTransform Trans = SkeletalMeshComponent->GetSocketTransform(TEXT("RightHand"));
	//		UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance()); 
	//		FVector Vec = Trans.GetLocation();

	//		if (nullptr != BowAnim)
	//		{
	//			BowAnim->SetHandTransform(Vec);
	//		}
	//	}
	//}
	//else if (false == BowStringCheck)
	//{
	//	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

	//	if (nullptr != BowAnim)
	//	{
	//		FVector Vec = FVector(27.662823, 0, 0);
	//		BowAnim->SetHandTransform(Vec);
	//	}
	//}
}

void UWeaponAction::WAndSButtonAction(float _Value)
{
	// 이동하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	// 블락 중일 땐 이동하지 않는다
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// 걷는다
		if (AnimState == CharacterAnimState::Idle || AnimState == CharacterAnimState::Walk)
		{
			IsForwardWalk = true;

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			AnimState = CharacterAnimState::Walk;
		}
		// 달린다
		else if (AnimState == CharacterAnimState::Run)
		{
			IsForwardWalk = true;
		}

		// 점프 하며 공중에 있는게 아니라면 이동 불가능
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
		// 앞뒤 입력이 없으면 false
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
	// 이동하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	// 블락 중일 땐 이동하지 않는다
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		// 걷는다
		if (AnimState == CharacterAnimState::Idle || AnimState == CharacterAnimState::Walk)
		{
			IsLeftWalk = true;

			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			AnimState = CharacterAnimState::Walk;
		}
		// 달린다
		else if (AnimState == CharacterAnimState::Run)
		{
			IsLeftWalk = true;
		}

		// 점프 하며 공중에 있는게 아니라면 이동 불가능
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
		// 좌우 입력이 없으면 false
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

		// 짧게 입력이 들어왔는지 확인
		if (nullptr != CurCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
		{
			// 구르면 안되는 상태
			switch (AnimState)
			{
			case CharacterAnimState::WalkJump:
			case CharacterAnimState::RunJump:
			case CharacterAnimState::Roll:
			case CharacterAnimState::EquipOrDisArmBow:
			case CharacterAnimState::EquipOrDisArmSwordAndShield:
			case CharacterAnimState::Attack:
			case CharacterAnimState::ParryorFire:
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}
			
			// 구른다
			IsRollMove = true;
			AnimState = CharacterAnimState::Roll;
		}

		// 입력이 멈추면 누른 시간 0
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	}

	// 달리기와 구르면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	// 점프가 끝나고도 달리기 유지
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
		return;
		break;
	}

	// 누른 시간 체크 시작
	if (false == PressSpacebar)
	{
		PressSpacebar = true;
	}

	// 달리면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::AimOrBlock:
		return;
	}

	if (false == IsForwardWalk && false == IsLeftWalk)
	{
		PressSpacebarTime = 0;
		PressSpacebar = false;
		AnimState = CharacterAnimState::Idle;
		return;
	}

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// 달린다
		// 달리는 애니메이션이 빨리 재생된다???
		AnimState = CharacterAnimState::Run;

		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UWeaponAction::ShiftButtonAction()
{
	// 점프하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::Roll:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	// 달릴 때 점프
	if (CharacterAnimState::Run == AnimState)
	{
		AnimState = CharacterAnimState::RunJump;
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		CurCharacter->Jump();
	}
	// 걸을 때, 가만히 있을 때 점프
	else
	{
		AnimState = CharacterAnimState::WalkJump;
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	CurCharacter->GetCharacterMovement()->AirControl = 0.3f;
}

void UWeaponAction::AttackAction()
{
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		AnimState = CharacterAnimState::Attack;
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		// 조준 중이며 화살이 준비 된 상태
		if (CharacterAnimState::AimOrBlock == AnimState && true == ArrowReady)
		{
			AnimState = CharacterAnimState::ParryorFire;
		}
		// 조준 중이며 화살이 준비 안된 상태
		else if (CharacterAnimState::AimOrBlock == AnimState && false == EarlyArrowCheck)
		{
			EarlyArrowCheck = true;
		}
		// 조준이 아닌 일반 공격인 상태
		else if (CharacterAnimState::AimOrBlock != AnimState && false == ArrowReady)
		{
			AnimState = CharacterAnimState::Attack;
		}
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState)
		{
			AttackCheck = true;
		}

		AnimState = CharacterAnimState::Attack;
	}
}

void UWeaponAction::AimorBlockAtion(float _Value)
{
	if (0 == _Value)
	{
		if (CharacterAnimState::AimOrBlock == AnimState)
		{
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			AnimState = CharacterAnimState::Idle;
		}

		return;
	}

	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::Run:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		return;
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		AnimState = CharacterAnimState::AimOrBlock;
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = AimorBlockSpeed;
		AnimState = CharacterAnimState::AimOrBlock;
	}
}
