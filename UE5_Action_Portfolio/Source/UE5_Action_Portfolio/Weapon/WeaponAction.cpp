// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon/WeaponAction.h"
#include "Global/Data/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Global/GlobalGameInstance.h"
#include "Global/GlobalCharAnimInstance.h"
#include "Global/GlobalCharacter.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"

UWeaponAction::UWeaponAction()
{	
	static ConstructorHelpers::FClassFinder<AArrow> BPArrow(TEXT("/Script/Engine.Blueprint'/Game/BluePrint/BP_Arrow.BP_Arrow_C'"));

	if (BPArrow.Succeeded())
	{
		ArrowClass = BPArrow.Class;
	}
}

void UWeaponAction::BeginPlay()
{
}

void UWeaponAction::Tick(float _DeltaTime)
{
	PressSpaceBarCkeckAndRoll(_DeltaTime);

	if (true == IsLockOn && (CharacterAnimState::Idle == AnimState || CharacterAnimState::Walk == AnimState))
	{
		AnimState = CharacterAnimState::LockOnIdle;
	}
	else if (false == IsLockOn && CharacterAnimState::LockOnIdle == AnimState)
	{
		AnimState = CharacterAnimState::Idle;
	}

	// ���½� �޸��� �� �� �ٽ� ȸ���� �������� �ð�
	if (true == LockOnCheck)
	{
		LockOnAfterRunTime += _DeltaTime;
	}

	if (nullptr != ReadyArrow)
	{
		AGlobalCharacter* GlobalChar = Cast<AGlobalCharacter>(CurCharacter);

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

void UWeaponAction::SetIsLockOn(bool _Value)
{
	IsLockOn = _Value;
}

bool UWeaponAction::GetIsAimOn()
{
	return IsAimOn;
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

void UWeaponAction::ChangeCollisionAttackType()
{
	AGlobalCharacter* GlobalChar = Cast<AGlobalCharacter>(CurCharacter);

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
			//����� �浹 ���� ���� �� ���� ����
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
	AGlobalCharacter* GlobalChar = Cast<AGlobalCharacter>(CurCharacter);

	if (EWeaponType::UnArmed == WeaponType)
	{
		GlobalChar->UnArmedWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState || CharacterAnimState::ParryorFire == AnimState)
		{
			GlobalChar->SwordWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
		}
		else if (CharacterAnimState::AimOrBlock == AnimState)
		{
			//����� �浹 ���� ���� �� ���� ����
		}
	}
}

void UWeaponAction::ArrowSpawn()
{
	FVector SpawnPos = CurCharacter->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
	FRotator SpawnRot = { -90, 0, 0 };

	FActorSpawnParameters SParam;
	// �浹�� ��ó �浹���� �ʴ� ��ġ�� �����ȴ�.
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
	// Idle�� LockOnIdle�� ���� ���� ����, ��� ���� �־�� UnArmed ����
	if ((CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState) || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// ������� ���� AnimInstance
	UGlobalCharAnimInstance* Ptr = Cast<UGlobalCharAnimInstance>(CurCharacter->GetMesh()->GetAnimInstance());

	if (nullptr == Ptr)
	{
		return;
	}

	// �����ϰ� �ִ� ��� Ȱ�� ��
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmBow;
	}
	// �����ϰ� �ִ� ��� Į�� ��
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponAction::ChangeSetBow()
{
	// Idle�� LockOnIdle�� ������ ���´� ����
	if (CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState)
	{
		return;
	}
	// ���� ����� ����� ��� ���� ����
	else if (EWeaponType::Bow == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// �ٸ� ����� ����� ��ü�ϰ� ���
	else if (EWeaponType::Sword == WeaponType)
	{
		// ��� ���� �ִϸ��̼��� �ȵ�
		ChangeSetUnArmed();
		SwordAndSheiledToBow = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmBow;

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponAction::ChangeSetSwordAndSheiled()
{
	// Idle�� LockOnIdle�� ������ ���´� ����
	if (CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState)
	{
		return;
	}
	//���� ����� ����� ��� ���� ����
	else if (EWeaponType::Sword == WeaponType)
	{
		ChangeSetUnArmed();
		return;
	}
	// �ٸ� ����� ����� ��ü�ϰ� ���
	else if (EWeaponType::Bow == WeaponType)
	{
		// ��� ���� �ִϸ��̼��� �ȵ�
		ChangeSetUnArmed();
		BowToSwordAndSheiled = true;
		return;
	}

	AnimState = CharacterAnimState::EquipOrDisArmSwordAndShield;

	ChangeWeapon(TEXT("SwordAndShield"));
}

void UWeaponAction::PressSpaceBarCkeckAndRoll(float _DeltaTime)
{
	// SpaceBar ���� �ð� üũ
	if (true == PressSpacebar)
	{
		PressSpacebarTime += _DeltaTime;
	}

	if (true == IsRollMove)
	{
		// �̷��� if�� 4���� �ƴ� FVector�� �� �� �ִ°�???
		// ���� ������
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

		// �����̱�
		FVector DeltaLocation = CurCharacter->GetActorRotation().Vector();

		DeltaLocation.X = 600 * _DeltaTime;
		DeltaLocation.Y = 0;

		CurCharacter->AddActorLocalOffset(DeltaLocation, true);
	}
}

bool UWeaponAction::LockOnAfterRun()
{
	if (LockOnAfterRunCount < LockOnAfterRunTime)
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
	// �̵��ϸ� �ȵǴ� ����
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
	MoveXValue = 0;
		return;
		break;
	}

	MoveXValue = _Value;

	// ��� ���� �� �̵����� �ʴ´�
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// �̵��Ѵ�
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			AnimState = CharacterAnimState::Walk;
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
			if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
				break;
			}

			// ���� �� �޸��� �� �Ŀ��� �������θ� �ȴ´�.
			if (true == LockOnCheck)
			{
				AnimState = CharacterAnimState::LockOnForward;
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnBackward;
			}
			else if (1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnForward;
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
		// �յ� �Է��� ������ false
		if (0.f == _Value)
		{
			IsForwardWalk = false;
			IsBackwardWalk = false;
		}

		if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnForward == AnimState || CharacterAnimState::LockOnBackward == AnimState) && (false == IsLeftWalk && false == IsRightWalk))
		{
			if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
			else if (true == IsLockOn)
			{
				AnimState = CharacterAnimState::LockOnIdle;
			}
		}
	}
}

void UWeaponAction::DAndAButtonAction(float _Value)
{
	// �̵��ϸ� �ȵǴ� ����
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
	MoveYValue = 0;
		return;
		break;
	}

	MoveYValue = _Value;

	// ��� ���� �� �̵����� �ʴ´�
	if (EWeaponType::Sword == WeaponType && CharacterAnimState::AimOrBlock == AnimState)
	{
		return;
	}

	// �̵��Ѵ�
	if (nullptr != CurCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			AnimState = CharacterAnimState::Walk;
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
			if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
				break;
			}

			// ���� �� �޸��� �� �Ŀ��� �������θ� �ȴ´�.
			if (true == LockOnCheck)
			{
				AnimState = CharacterAnimState::LockOnForward;
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnLeft;
			}
			else if (1.f == _Value)
			{
				AnimState = CharacterAnimState::LockOnRight;
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
		// �¿� �Է��� ������ false
		if (0.f == _Value)
		{
			IsLeftWalk = false;
			IsRightWalk = false;
		}

		if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnLeft == AnimState || CharacterAnimState::LockOnRight == AnimState) && (false == IsForwardWalk && false == IsBackwardWalk))
		{
			if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
			else if (true == IsLockOn)
			{
				AnimState = CharacterAnimState::LockOnIdle;
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
				LockOnCheck = true;
				AnimState = CharacterAnimState::LockOnIdle;
			}
			else if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
		}

		// ª�� �Է��� ���Դ��� Ȯ��
		if (nullptr != CurCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
		{
			// ������ �ȵǴ� ����
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
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}
			
			// ������
			IsRollMove = true;
			AnimState = CharacterAnimState::Roll;
		}

		// �Է��� ���߸� ���� �ð� 0
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	}

	// �޸���� ������ �ȵǴ� ����
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
		PressSpacebarTime = 0;
		PressSpacebar = false;
		return;
	// ������ ������ �޸��� ����
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::RunJump:
		return;
		break;
	}

	// ���� �ð� üũ ����
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
				AnimState = CharacterAnimState::LockOnIdle;
			}
			else if (false == IsLockOn)
			{
				AnimState = CharacterAnimState::Idle;
			}
		}

		return;
	}

	// �޸��� �ȵǴ� ����
	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::AimOrBlock:
	case CharacterAnimState::LockOnIdle:
		return;
	}

	if (nullptr != CurCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// �޸���
		if (true == IsLockOn)
		{
			LockOnAfterRunTime = 0.f;
			CurCharacter->bUseControllerRotationYaw = false;
			CurCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		}

		AnimState = CharacterAnimState::Run;

		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UWeaponAction::ShiftButtonAction()
{
	// �����ϸ� �ȵǴ� ����
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
		return;
		break;
	}

	// �޸� �� ����
	if (CharacterAnimState::Run == AnimState)
	{
		AnimState = CharacterAnimState::RunJump;
		CurCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		CurCharacter->Jump();
	}
	// ���� ��, ������ ���� �� ����
	else
	{
		AnimState = CharacterAnimState::WalkJump;
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
	// ���� �ϸ� �ȵǴ� ����
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
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		AnimState = CharacterAnimState::Attack;
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		// ���� ���̸� ȭ���� �غ� �� ����
		if (CharacterAnimState::AimOrBlock == AnimState && true == ArrowReady)
		{
			AnimState = CharacterAnimState::ParryorFire;
		}
		// ���� ���̸� ȭ���� �غ� �ȵ� ����
		else if (CharacterAnimState::AimOrBlock == AnimState && false == EarlyArrowCheck)
		{
			EarlyArrowCheck = true;
		}
		// ������ �ƴ� �Ϲ� ������ ����
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
	// ���� �� ���� ���� �Է� ���� �� false �Է� ���� �� true
	// true�� �� �޴� ������ ����
	if (0 == _Value)
	{
		IsAimOn = false;

		IsBlock = false;

		if (CharacterAnimState::AimOrBlock == AnimState)
		{
			CurCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			AnimState = CharacterAnimState::Idle;
		}

		if (EWeaponType::Bow == WeaponType && CharacterAnimState::Attack == AnimState)
		{
			return;
		}

		AGlobalCharacter* character = Cast<AGlobalCharacter>(CurCharacter);

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

		IsAimOn = true;
	}
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

void UWeaponAction::GotHit()
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
		AnimState = CharacterAnimState::GotHit;

		break;
	case CharacterAnimState::AimOrBlock:
		if (EWeaponType::Bow == WeaponType)
		{
			AnimState = CharacterAnimState::AimOrBlockGotHit;

			AGlobalCharacter* character = Cast<AGlobalCharacter>(CurCharacter);

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
		else if (EWeaponType::Sword == WeaponType)
		{
			AnimState = CharacterAnimState::AimOrBlockGotHit;
		}
		break;
	default:
		break;
	}
}

void UWeaponAction::Death()
{
		AnimState = CharacterAnimState::Death;

}
