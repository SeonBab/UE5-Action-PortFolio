#include "Weapon/WeaponAction.h"
#include "Global/Data/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global/GlobalGameInstance.h"
#include "Character/MainCharAnimInstance.h"
#include "Character/WeaponCharacter.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"

UWeaponAction::UWeaponAction()
{	

}

void UWeaponAction::BeginPlay()
{
	if (nullptr == CurCharacter)
	{
		return;
	}

	UGlobalGameInstance* Inst = CurCharacter->GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (nullptr == Inst)
	{
		return;
	}

	TSubclassOf<UObject> Arrow = Inst->GetSubClass(TEXT("Arrow"));

	if (nullptr == Arrow)
	{
		return;
	}

	ArrowClass = Arrow;
}

void UWeaponAction::Tick(float _DeltaTime)
{
	PressSpaceBarCkeckAndRoll(_DeltaTime);

	if ((true == IsLockOn || true == IsAimOn) && (CharacterAnimState::Idle == AnimState || CharacterAnimState::Walk == AnimState))
	{
		SetAnimState(CharacterAnimState::LockOnIdle);
	}
	else if ((false == IsLockOn && false == IsAimOn) && CharacterAnimState::LockOnIdle == AnimState)
	{
		SetAnimState(CharacterAnimState::Idle);
	}

	// 락온시 달리고 난 후 다시 회전할 때까지의 시간
	if (true == LockOnCheck)
	{
		LockOnAfterRunTime += _DeltaTime;

		if (LockOnAfterRunCount <= LockOnAfterRunTime || CharacterAnimState::Attack == AnimState)
		{
			LockOnAfterRunTime = 0.f;
			LockOnCheck = false;
		}
	}

	if (nullptr != ReadyArrow)
	{
		AWeaponCharacter* GlobalChar = Cast<AWeaponCharacter>(CurCharacter);

		if (nullptr == GlobalChar)
		{
			return;
		}

		FVector JointPos = GlobalChar->GetBowJointLocation();

		ReadyArrow->ArrowReRoad(CurCharacter, JointPos, _DeltaTime);
	}
}

void UWeaponAction::SetCurCharacter(ACharacter* _CurChar)
{
	CurCharacter = _CurChar;
}

void UWeaponAction::SetCharacterAirControl(float _Value)
{
	CurCharacter->GetCharacterMovement()->AirControl = _Value;
}

EWeaponType UWeaponAction::GetWeaponType()
{
	return WeaponType;
}

bool UWeaponAction::GetAttackCheck()
{
	return AttackCheck;
}

void UWeaponAction::SetAttackCheck(bool _Value)
{
	AttackCheck = _Value;
}

void UWeaponAction::SetIsLockOn(bool _Value)
{
	IsLockOn = _Value;
}

bool UWeaponAction::GetIsAimOn()
{
	return IsAimOn;
}

bool UWeaponAction::GetIsRollMove()
{
	return IsRollMove;
}

void UWeaponAction::SetAttackType(FName _AttackType)
{
	AttackType = _AttackType;
}

void UWeaponAction::SetIsBlock(bool _Value)
{
	IsBlock = _Value;
}

bool UWeaponAction::GetIsBlock()
{
	return IsBlock;
}

void UWeaponAction::SetIsParry(bool _Value)
{
	IsParry = _Value;
}

bool UWeaponAction::GetIsParry()
{
	return IsParry;
}

bool UWeaponAction::GetIsInvincibility()
{
	return IsInvincibility;
}

void UWeaponAction::ChangeCollisionAttackType()
{
	AWeaponCharacter* GlobalChar = Cast<AWeaponCharacter>(CurCharacter);

	if (nullptr == GlobalChar)
	{
		return;
	}

	if (TEXT("") == AttackType)
	{
		return;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		GlobalChar->UnArmedWeaponMesh->SetCollisionProfileName(AttackType, true);
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState || CharacterAnimState::ParryorFire == AnimState)
		{
			GlobalChar->SwordWeaponMesh->SetCollisionProfileName(AttackType, true);
		}
		else if (CharacterAnimState::AimOrBlock == AnimState)
		{
			IsBlock = true;
		}
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		if (nullptr == ReadyArrow)
		{
			return;
		}

		ReadyArrow->ArrowChangeCollision(AttackType);
	}
}

void UWeaponAction::ChangeNoCollision()
{
	AWeaponCharacter* GlobalChar = Cast<AWeaponCharacter>(CurCharacter);

	if (EWeaponType::UnArmed == WeaponType)
	{
		GlobalChar->UnArmedWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		GlobalChar->SwordWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
		IsBlock = false;
	}
}

void UWeaponAction::ArrowSpawn()
{
	FVector SpawnPos = CurCharacter->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
	FRotator SpawnRot = { -90, 0, 0 };

	FActorSpawnParameters SParam;
	// 충돌시 근처 충돌하지 않는 위치에 스폰된다.
	SParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	if (nullptr == ArrowClass)
	{
		return;
	}

	ReadyArrow = CurCharacter->GetWorld()->UWorld::SpawnActor<AArrow>(ArrowClass, SpawnPos, SpawnRot, SParam);
}

AArrow* UWeaponAction::GetReadyArrow()
{
	return ReadyArrow;
}

void UWeaponAction::SetnullReadyArrow()
{
	ReadyArrow = nullptr;
}

CharacterAnimState UWeaponAction::GetAnimState()
{
	return AnimState;
}

int UWeaponAction::GetAnimStateToInt()
{
	return static_cast<int>(AnimState);
}

void UWeaponAction::SetAnimState(CharacterAnimState _State)
{
	AnimState = _State;
}

void UWeaponAction::IsRollMoveToFalse()
{
	IsRollMove = false;
	IsInvincibility = false;
}

void UWeaponAction::ChangeWeapon(FName _Weapon)
{
	UMainCharAnimInstance* Ptr = Cast<UMainCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

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

	Ptr->SetAllAnimation(FindAnimationData->AllAnimations);
	WeaponType = FindAnimationData->Type;
}

void UWeaponAction::ChangeSetUnArmed()
{
	// Idle과 LockOnIdle만 무기 변경 가능, 장비를 끼고 있어야 UnArmed 가능
	if ((CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState) || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// 역재생을 위한 AnimInstance
	UMainCharAnimInstance* Ptr = Cast<UMainCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	// 착용하고 있던 장비가 활일 때
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		SetAnimState(CharacterAnimState::EquipOrDisArmBow);
	}
	// 착용하고 있던 장비가 칼일 때
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		SetAnimState(CharacterAnimState::EquipOrDisArmSwordAndShield);
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponAction::ChangeSetBow()
{
	// Idle과 LockOnIdle을 제외한 상태는 리턴
	if (CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState)
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

	SetAnimState(CharacterAnimState::EquipOrDisArmBow);

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponAction::ChangeSetSwordAndSheiled()
{
	// Idle과 LockOnIdle을 제외한 상태는 리턴
	if (CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState)
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

	SetAnimState(CharacterAnimState::EquipOrDisArmSwordAndShield);

	ChangeWeapon(TEXT("SwordAndShield"));
}

void UWeaponAction::PressSpaceBarCkeckAndRoll(float _DeltaTime)
{
	// SpaceBar 누른 시간 체크
	if (true == PressSpacebar)
	{
		PressSpacebarTime += _DeltaTime;
	}

	if (true == IsRollMove)
	{
		// 이렇게 if문 4개가 아닌 FVector로 할 수 있는가???
		// 방향 돌리기
		if ((true == IsLockOn && false == LockOnCheck) || (EWeaponType::Bow == WeaponType && CharacterAnimState::AimOrBlock == AnimState))
		{
			FRotator Rotation;
			FVector Angle;

			if (true == IsForwardWalk)
			{
				Angle = CurCharacter->GetActorForwardVector();
				Angle.Z = 0;
			}
			if (true == IsBackwardWalk)
			{
				Angle = -CurCharacter->GetActorForwardVector();
				Angle.Z = 0;
			}
			if (true == IsLeftWalk)
			{
				Angle += -CurCharacter->GetActorRightVector();
				Angle.Z = 0;
			}
			if (true == IsRightWalk)
			{
				Angle += CurCharacter->GetActorRightVector();
				Angle.Z = 0;
			}

			Rotation = Angle.Rotation();
			CurCharacter->SetActorRotation(Rotation);
		}

		// 움직이기
		FVector DeltaLocation = CurCharacter->GetActorRotation().Vector();

		DeltaLocation.X = 500 * _DeltaTime;
		DeltaLocation.Y = 0;

		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
		IsInvincibility = true;
	}
}

bool UWeaponAction::LockOnAfterRun()
{
	if (LockOnAfterRunCount <= LockOnAfterRunTime)
	{
		LockOnCheck = false;
		LockOnAfterRunTime = 0.f;

		return true;
	}
	else if (CharacterAnimState::Attack == AnimState)
	{
		LockOnCheck = false;
		LockOnAfterRunTime = 0.f;

		return true;
	}

	return false;
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
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
	MoveXValue = 0;
		return;
		break;
	}

	MoveXValue = _Value;

	// 블락 중일 땐 이동하지 않는다
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			SetAnimState(CharacterAnimState::Walk);
			break;
		case CharacterAnimState::Walk:
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		case CharacterAnimState::Run:
			break;
		case CharacterAnimState::LockOnIdle:
		case CharacterAnimState::LockOnBackward:
		case CharacterAnimState::LockOnForward:
		case CharacterAnimState::LockOnLeft:
		case CharacterAnimState::LockOnRight:
			if (false == IsLockOn && false == IsAimOn)
			{
				SetAnimState(CharacterAnimState::Walk);
			}
			// 락온 중 달리고 난 후에는 정면으로만 걷는다.
			else if (true == LockOnCheck)
			{
				SetAnimState(CharacterAnimState::LockOnForward);
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				SetAnimState(CharacterAnimState::LockOnBackward);
			}
			else if (1.f == _Value)
			{
				SetAnimState(CharacterAnimState::LockOnForward);
			}
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
			break;
		case CharacterAnimState::LockOnForwardRun:
			break;
		}


		if (-1.f == _Value)
		{
			IsForwardWalk = false;
			IsBackwardWalk = true;
		}
		else if (1.f == _Value)
		{
			IsForwardWalk = true;
			IsBackwardWalk = false;
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
			IsBackwardWalk = false;
		}

		if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnForward == AnimState || CharacterAnimState::LockOnBackward == AnimState) && (false == IsLeftWalk && false == IsRightWalk))
		{
			if (false == IsLockOn)
			{
				SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == IsLockOn)
			{
				SetAnimState(CharacterAnimState::LockOnIdle);
			}
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
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
	MoveYValue = 0;
		return;
		break;
	}

	MoveYValue = _Value;

	// 블락 중일 땐 이동하지 않는다
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// 이동한다
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			SetAnimState(CharacterAnimState::Walk);
			break;
		case CharacterAnimState::Walk:
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			break;
		case CharacterAnimState::Run:
			break;
		case CharacterAnimState::LockOnIdle:
		case CharacterAnimState::LockOnForward:
		case CharacterAnimState::LockOnBackward:
		case CharacterAnimState::LockOnLeft:
		case CharacterAnimState::LockOnRight:
			if (false == IsLockOn && false == IsAimOn)
			{
				SetAnimState(CharacterAnimState::Walk);
			}
			// 락온 중 달리고 난 후에는 정면으로만 걷는다.
			else if (true == LockOnCheck)
			{
				SetAnimState(CharacterAnimState::LockOnForward);
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				SetAnimState(CharacterAnimState::LockOnLeft);
			}
			else if (1.f == _Value)
			{
				SetAnimState(CharacterAnimState::LockOnRight);
			}
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
			break;
		case CharacterAnimState::LockOnForwardRun:
			break;
		}

		if (-1.f == _Value)
		{
			IsLeftWalk = true;
			IsRightWalk = false;
		}
		else if (1.f == _Value)
		{
			IsLeftWalk = false;
			IsRightWalk = true;
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
			IsRightWalk = false;
		}

		if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnLeft == AnimState || CharacterAnimState::LockOnRight == AnimState) && (false == IsForwardWalk && false == IsBackwardWalk))
		{
			if (false == IsLockOn)
			{
				SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == IsLockOn)
			{
				SetAnimState(CharacterAnimState::LockOnIdle);
			}
		}
	}
}

void UWeaponAction::RollorRunAction(float _Value)
{
	if (0.f == _Value)
	{
		if (AnimState == CharacterAnimState::Run)
		{
			if (true == IsLockOn)
			{
				SetAnimState(CharacterAnimState::LockOnIdle);
			}
			else if (false == IsLockOn)
			{
				SetAnimState(CharacterAnimState::Idle);
			}
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
			case CharacterAnimState::GotHit:
			case CharacterAnimState::AimOrBlockGotHit:
			case CharacterAnimState::Death:
			case CharacterAnimState::Dizzy:
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}
			
			// 구른다
			IsRollMove = true;
			SetAnimState(CharacterAnimState::Roll);
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
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
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

	if (false == IsForwardWalk && false == IsBackwardWalk && false == IsLeftWalk && false == IsRightWalk)
	{
		PressSpacebarTime = 0;
		PressSpacebar = false;

		if (CharacterAnimState::AimOrBlock != AnimState)
		{
			if (true == IsLockOn)
			{
				LockOnCheck = false;
				LockOnAfterRunTime = 0.f;
				SetAnimState(CharacterAnimState::LockOnIdle);
			}
			else if (false == IsLockOn)
			{
				SetAnimState(CharacterAnimState::Idle);
			}
		}

		return;
	}

	// 달리면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::AimOrBlock:
	case CharacterAnimState::LockOnIdle:
		return;
	}

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// 달린다
		if (true == IsLockOn)
		{
			LockOnCheck = true;
			LockOnAfterRunTime = 0.f;
			CurCharacter->bUseControllerRotationYaw = false;
			CurCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		}

		SetAnimState(CharacterAnimState::Run);

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
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
		return;
		break;
	}

	// 달릴 때 점프
	if (CharacterAnimState::Run == AnimState)
	{
		SetAnimState(CharacterAnimState::RunJump);
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		CurCharacter->Jump();
	}
	// 걸을 때, 가만히 있을 때 점프
	else
	{
		SetAnimState(CharacterAnimState::WalkJump);
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (true == LockOnCheck)
	{
		LockOnAfterRunTime = 0.f;
	}

	CurCharacter->GetCharacterMovement()->AirControl = 0.3f;
}

void UWeaponAction::AttackAction()
{
	// 공격 하면 안되는 상태
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::ParryorFire:
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		SetAnimState(CharacterAnimState::Attack);
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		// 조준 중이며 화살이 준비 된 상태
		if (CharacterAnimState::AimOrBlock == AnimState && true == ArrowReady)
		{
			SetAnimState(CharacterAnimState::ParryorFire);
			ArrowReady = false;
		}
		// 조준 중이며 화살이 준비 안된 상태
		else if (CharacterAnimState::AimOrBlock == AnimState && false == EarlyArrowCheck)
		{
			EarlyArrowCheck = true;
		}
		// 조준이 아닌 일반 공격인 상태
		else if (CharacterAnimState::AimOrBlock != AnimState && false == ArrowReady)
		{
			SetAnimState(CharacterAnimState::Attack);
		}
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState)
		{
			AttackCheck = true;
		}

		SetAnimState(CharacterAnimState::Attack);
	}
}

void UWeaponAction::AimorBlockAtion(float _Value)
{
	if (0 == _Value)
	{
		IsAimOn = false;
		IsBlock = false;

		if (CharacterAnimState::AimOrBlock == AnimState)
		{
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			SetAnimState(CharacterAnimState::Idle);
		}

		if (EWeaponType::Bow == WeaponType && CharacterAnimState::Attack == AnimState)
		{
			return;
		}

		if (nullptr == CurCharacter)
		{
			return;
		}

		AWeaponCharacter* character = Cast<AWeaponCharacter>(CurCharacter);

		UBowAnimInstance* BowAnim;

		if (nullptr == character && false == character->IsValidLowLevel())
		{
			return;
		}

		USkeletalMeshComponent* BowMesh = character->BowWeaponMesh;

		if (nullptr == BowMesh)
		{
			return;
		}

		BowAnim = Cast<UBowAnimInstance>(BowMesh->GetAnimInstance());

		if (nullptr == BowAnim)
		{
			return;
		}
		
		BowAnim->SetBowChordCheck(false);

		if (nullptr == ReadyArrow)
		{
			return;
		}

		ArrowReady = false;
		ReadyArrow->Destroy();

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
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		return;
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		SetAnimState(CharacterAnimState::AimOrBlock);
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = AimorBlockSpeed;
		SetAnimState(CharacterAnimState::AimOrBlock);

		IsAimOn = true;
	}
}

void UWeaponAction::ParryAction()
{
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
		return;
		break;
	}

	if (EWeaponType::Sword != WeaponType)
	{
		return;
	}

	SetAnimState(CharacterAnimState::ParryorFire);

}

float UWeaponAction::GetMoveXValue()
{
	return MoveXValue;
}

float UWeaponAction::GetMoveYValue()
{
	return MoveYValue;
}

bool UWeaponAction::GetIsLockOn()
{
	return IsLockOn;
}

bool UWeaponAction::GetIsMove()
{
	bool IsMove = false;

	switch (AnimState)
	{
	case CharacterAnimState::LockOnIdle:
	case CharacterAnimState::LockOnBackward:
	case CharacterAnimState::LockOnForward:
	case CharacterAnimState::LockOnLeft:
	case CharacterAnimState::LockOnRight:
		IsMove = true;
		break;
	default:
		IsMove = false;
		break;
	}

	return IsMove;
}

bool UWeaponAction::GetLockOnCheck()
{
	return LockOnCheck;
}

void UWeaponAction::SetLockOnCheck(bool _Value)
{
	LockOnCheck = _Value;
}

void UWeaponAction::GotHit(FVector _Value)
{
	//UMaterialInterface* CurMaterial = CurCharacter->GetMesh()->GetMaterial(0);

	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::Walk:
	case CharacterAnimState::LockOnIdle:
	case CharacterAnimState::LockOnBackward:
	case CharacterAnimState::LockOnForward:
	case CharacterAnimState::LockOnLeft:
	case CharacterAnimState::LockOnRight:
		SetAnimState(CharacterAnimState::GotHit);

		if (true == IsLockOn)
		{
			break;
		}

		CurCharacter->SetActorRotation(_Value.ToOrientationQuat());
		break;
	case CharacterAnimState::AimOrBlock:
		if (EWeaponType::Bow == WeaponType)
		{
			SetAnimState(CharacterAnimState::AimOrBlockGotHit);

			UBowAnimInstance* BowAnim;

			if (nullptr == CurCharacter)
			{
				return;
			}

			AWeaponCharacter* character = Cast<AWeaponCharacter>(CurCharacter);

			if (nullptr == character && false == character->IsValidLowLevel())
			{
				return;
			}

			USkeletalMeshComponent* BowMesh = character->BowWeaponMesh;

			if (nullptr == BowMesh)
			{
				return;
			}

			BowAnim = Cast<UBowAnimInstance>(BowMesh->GetAnimInstance());

			if (nullptr == BowAnim)
			{
				return;
			}

			BowAnim->SetBowChordCheck(false);

			if (nullptr == ReadyArrow)
			{
				return;
			}

			ArrowReady = false;
			ReadyArrow->Destroy();
		}
		else if (EWeaponType::Sword == WeaponType)
		{
			SetAnimState(CharacterAnimState::AimOrBlockGotHit);
		}
		break;
	default:
		break;
	}
}

void UWeaponAction::Death()
{
	// 검으로 공격중이면 안죽는다.
	SetAnimState(CharacterAnimState::Death);

	if (EWeaponType::Bow == WeaponType)
	{
		AWeaponCharacter* character = Cast<AWeaponCharacter>(CurCharacter);

		UBowAnimInstance* BowAnim;

		if (nullptr == character && false == character->IsValidLowLevel())
		{
			return;
		}

		USkeletalMeshComponent* BowMesh = character->BowWeaponMesh;

		if (nullptr == BowMesh)
		{
			return;
		}

		BowAnim = Cast<UBowAnimInstance>(BowMesh->GetAnimInstance());

		if (nullptr == BowAnim)
		{
			return;
		}

		BowAnim->SetBowChordCheck(false);

		if (nullptr == ReadyArrow)
		{
			return;
		}

		ArrowReady = false;
		ReadyArrow->Destroy();
	}
}
