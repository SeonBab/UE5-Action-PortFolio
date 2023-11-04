#include "Weapon/WeaponComponent.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"
#include "Engine/DamageEvents.h"
#include "Global/GlobalAICharacter.h"
#include "Global/GlobalAnimInstance.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/WeaponData.h"
#include "Global/Data/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

	ReadyArrow = nullptr;

	UnArmedWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("UnArmedMesh"));
	BowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BowMesh"));
	SwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SwordMesh"));
	ShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("ShiedlMesh"));

	BackBowWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackBowMesh"));
	BackSwordWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackSwordMesh"));
	BackShieldWeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BackShiedlMesh"));

	UnArmedWeaponMesh->SetGenerateOverlapEvents(true);
	BowWeaponMesh->SetGenerateOverlapEvents(true);
	SwordWeaponMesh->SetGenerateOverlapEvents(true);
}

void UWeaponComponent::SetMeshAttach(USkeletalMeshComponent* _Mesh)
{
	if (false == IsValid(_Mesh))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	BackBowWeaponMesh->SetupAttachment(_Mesh, TEXT("BackBow"));
	BackSwordWeaponMesh->SetupAttachment(_Mesh, TEXT("BackSword"));
	BackShieldWeaponMesh->SetupAttachment(_Mesh, TEXT("BackShield"));

	UnArmedWeaponMesh->SetupAttachment(_Mesh, TEXT("LeftHandSoket"));
	BowWeaponMesh->SetupAttachment(_Mesh, TEXT("LeftHandSoket"));
	SwordWeaponMesh->SetupAttachment(_Mesh, TEXT("RightHandSoket"));
	ShieldWeaponMesh->SetupAttachment(_Mesh, TEXT("LeftHandShield"));

	UnArmedWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponent::WeaponBeginOverlap);
	BowWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponent::WeaponBeginOverlap);
	SwordWeaponMesh->OnComponentBeginOverlap.AddDynamic(this, &UWeaponComponent::WeaponBeginOverlap);
}

void UWeaponComponent::ChangeCollisionAttackType()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FName AttackType= GlobalAICharacter->GetAttackTypeTag();

	if (TEXT("") == AttackType)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> TEXT("") == AttackType"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	if (EWeaponType::UnArmed == WeaponType)
	{
		UnArmedWeaponMesh->SetCollisionProfileName(AttackType, true);
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState || CharacterAnimState::ParryorFire == AnimState)
		{
			SwordWeaponMesh->SetCollisionProfileName(AttackType, true);
		}
		else if (CharacterAnimState::AimOrBlock == AnimState)
		{
			IsBlock = true;
		}
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		if (false == IsValid(ReadyArrow))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return;
		}

		ReadyArrow->ArrowChangeCollision(AttackType);
	}
}

void UWeaponComponent::OverlapEnd()
{
	// ������ �迭 �ʱ�ȭ
	OverlapArray.Empty();

	// �ݸ��� ����
	if (EWeaponType::UnArmed == WeaponType)
	{
		UnArmedWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		SwordWeaponMesh->SetCollisionProfileName(TEXT("NoCollision"), true);
		IsBlock = false;
	}
}

void UWeaponComponent::ArrowSpawn()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FVector SpawnPos = GlobalAICharacter->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
	FRotator SpawnRot = { -90, 0, 0 };

	FActorSpawnParameters SParam;
	// �浹�� ��ó �浹���� �ʴ� ��ġ�� �����ȴ�.
	SParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	if (false == IsValid(ArrowClass))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ReadyArrow = GlobalAICharacter->GetWorld()->UWorld::SpawnActor<AArrow>(ArrowClass, SpawnPos, SpawnRot, SParam);
}

void UWeaponComponent::SetCharacterAirControl(float _Value)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GlobalAICharacter->GetCharacterMovement()->AirControl = _Value;
}

void UWeaponComponent::SetAttackCheck(bool _Value)
{
	AttackCheck = _Value;
}

void UWeaponComponent::SetIsLockOn(bool _Value)
{
	IsLockOn = _Value;
}

void UWeaponComponent::SetIsBlock(bool _Value)
{
	IsBlock = _Value;
}

void UWeaponComponent::SetIsParry(bool _Value)
{
	IsParry = _Value;
}

void UWeaponComponent::SetnullReadyArrow()
{
	ReadyArrow = nullptr;
}

void UWeaponComponent::IsRollMoveToFalse()
{
	IsRollMove = false;

	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GlobalAICharacter->SetIsInvincibility(false);

	IsForwardWalk = false;
	IsBackwardWalk = false;
	IsLeftWalk = false;
	IsRightWalk = false;

	CharacterAnimState AnimState = GetAnimState();

	if ((CharacterAnimState::Walk == AnimState || CharacterAnimState::LockOnForward == AnimState || CharacterAnimState::LockOnBackward == AnimState) && (false == IsLeftWalk && false == IsRightWalk))
	{
		if (false == IsLockOn)
		{
			GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
		}
		else if (true == IsLockOn)
		{
			GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnIdle);
		}
	}
}

void UWeaponComponent::ChangeWeapon(FName _Weapon)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalAnimInstance* Ptr = Cast<UGlobalAnimInstance>(GlobalAICharacter->GetMesh()->GetAnimInstance());

	if (false == IsValid(Ptr))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalGameInstance* Instacne = GlobalAICharacter->GetWorld()->GetGameInstance<UGlobalGameInstance>();

	const struct FAnimaitionData* FindAnimationData = Instacne->GetAnimaitionDatas(_Weapon);

	if (nullptr == FindAnimationData)
	{
		return;
	}

	Ptr->SetAllAnimation(FindAnimationData->AllAnimations);
	WeaponType = FindAnimationData->Type;
}

void UWeaponComponent::ChangeSetUnArmed()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	// Idle�� LockOnIdle�� ���� ���� ����, ��� ���� �־�� UnArmed ����
	if ((CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState) || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// ������� ���� AnimInstance
	UGlobalAnimInstance* Ptr = Cast<UGlobalAnimInstance>(GlobalAICharacter->GetMesh()->GetAnimInstance());

	if (false == IsValid(Ptr))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// �����ϰ� �ִ� ��� Ȱ�� ��
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		GlobalAICharacter->SetAnimState(CharacterAnimState::EquipOrDisArmBow);
	}
	// �����ϰ� �ִ� ��� Į�� ��
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		GlobalAICharacter->SetAnimState(CharacterAnimState::EquipOrDisArmSwordAndShield);
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponComponent::ChangeSetBow()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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

	GlobalAICharacter->SetAnimState(CharacterAnimState::EquipOrDisArmBow);

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponComponent::ChangeSetSwordAndSheiled()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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

	GlobalAICharacter->SetAnimState(CharacterAnimState::EquipOrDisArmSwordAndShield);

	ChangeWeapon(TEXT("SwordAndShield"));

}

void UWeaponComponent::PressSpaceBarCkeckAndRoll(float _DeltaTime)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	// SpaceBar ���� �ð� üũ
	if (true == PressSpacebar)
	{
		PressSpacebarTime += _DeltaTime;
	}

	if (true == IsRollMove)
	{
		// ���� ������
		if ((true == IsLockOn && false == IsCameraStares) || (EWeaponType::Bow == WeaponType && CharacterAnimState::AimOrBlock == AnimState))
		{
			FRotator Rotation = FRotator::ZeroRotator;
			FVector Angle = FVector::ZeroVector;

			if (true == IsForwardWalk)
			{
				Angle = GlobalAICharacter->GetActorForwardVector();
			}
			else if (true == IsBackwardWalk)
			{
				Angle = -GlobalAICharacter->GetActorForwardVector();
			}
			if (true == IsLeftWalk)
			{
				Angle += -GlobalAICharacter->GetActorRightVector();
			}
			else if (true == IsRightWalk)
			{
				Angle += GlobalAICharacter->GetActorRightVector();
			}
			
			Angle.Z = 0;

			Rotation = Angle.Rotation();
			GlobalAICharacter->SetActorRotation(Rotation);
		}

		// �����̱�
		FVector DeltaLocation = GlobalAICharacter->GetActorRotation().Vector();

		DeltaLocation.X = 500 * _DeltaTime;
		DeltaLocation.Y = 0;

		GlobalAICharacter->AddActorLocalOffset(DeltaLocation, true);

		GlobalAICharacter->SetIsInvincibility(true);
	}
}

void UWeaponComponent::WAndSButtonAction(float _Value)
{
 	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
		return;
		break;
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
		MoveXValue = 0;
		IsForwardWalk = false;
		IsBackwardWalk = false;
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
	if (nullptr != GlobalAICharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			GlobalAICharacter->SetAnimState(CharacterAnimState::Walk);
			break;
		case CharacterAnimState::Walk:
			GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
				GlobalAICharacter->SetAnimState(CharacterAnimState::Walk);
			}
			// ���� �� �޸��� �� �Ŀ��� �������θ� �ȴ´�.
			else if (true == IsCameraStares)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnForward);
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnBackward);
			}
			else if (1.f == _Value)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnForward);
			}
			GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
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

		const FRotator Rotation = GlobalAICharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		GlobalAICharacter->AddMovementInput(Direction, _Value);

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
				GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == IsLockOn)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
		}
	}
}

void UWeaponComponent::DAndAButtonAction(float _Value)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	// �̵��ϸ� �ȵǴ� ����
	switch (AnimState)
	{
	case CharacterAnimState::Roll:
		return;
		break;
	case CharacterAnimState::WalkJump:
	case CharacterAnimState::EquipOrDisArmBow:
	case CharacterAnimState::EquipOrDisArmSwordAndShield:
	case CharacterAnimState::Attack:
	case CharacterAnimState::ParryorFire:
	case CharacterAnimState::GotHit:
	case CharacterAnimState::AimOrBlockGotHit:
	case CharacterAnimState::Death:
	case CharacterAnimState::Dizzy:
		MoveYValue = 0;
		IsLeftWalk = false;
		IsRightWalk = false;
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
	if (nullptr != GlobalAICharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			GlobalAICharacter->SetAnimState(CharacterAnimState::Walk);
			break;
		case CharacterAnimState::Walk:
			GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
				GlobalAICharacter->SetAnimState(CharacterAnimState::Walk);
			}
			// ���� �� �޸��� �� �Ŀ��� �������θ� �ȴ´�.
			else if (true == IsCameraStares)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnForward);
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnLeft);
			}
			else if (1.f == _Value)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnRight);
			}
			GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
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

		const FRotator Rotation = GlobalAICharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		GlobalAICharacter->AddMovementInput(Direction, _Value);
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
				GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == IsLockOn)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
		}
	}
}

void UWeaponComponent::RollorRunAction(float _Value)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	if (0.f == _Value)
	{
		if (AnimState == CharacterAnimState::Run)
		{
			if (true == IsLockOn)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			else if (false == IsLockOn)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
			}
		}

		// ª�� �Է��� ���Դ��� Ȯ��
		if (nullptr != GlobalAICharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
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
			case CharacterAnimState::Dizzy:
				PressSpacebarTime = 0;
				PressSpacebar = false;
				return;
			}

			// ������
			IsRollMove = true;
			GlobalAICharacter->SetAnimState(CharacterAnimState::Roll);
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
	case CharacterAnimState::Dizzy:
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
				IsCameraStares = false;
				LockOnAfterRunTime = 0.f;
				GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			else if (false == IsLockOn)
			{
				GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
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

	if (nullptr != GlobalAICharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// �޸���
		if (true == IsLockOn)
		{
			IsCameraStares = true;
			LockOnAfterRunTime = 0.f;
			GlobalAICharacter->bUseControllerRotationYaw = false;
			GlobalAICharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		}

		GlobalAICharacter->SetAnimState(CharacterAnimState::Run);

		GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UWeaponComponent::ShiftButtonAction()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
	case CharacterAnimState::Dizzy:
		return;
		break;
	}

	// �޸� �� ����
	if (CharacterAnimState::Run == AnimState)
	{
		GlobalAICharacter->SetAnimState(CharacterAnimState::RunJump);
		GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		GlobalAICharacter->Jump();
	}
	// ���� ��, ������ ���� �� ����
	else
	{
		GlobalAICharacter->SetAnimState(CharacterAnimState::WalkJump);
		GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (true == IsCameraStares)
	{
		LockOnAfterRunTime = 0.f;
	}

	GlobalAICharacter->GetCharacterMovement()->AirControl = 0.5f;
}

void UWeaponComponent::AttackAction()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
	case CharacterAnimState::Dizzy:
		return;
		break;
	}

	if (EWeaponType::UnArmed == WeaponType)
	{
		GlobalAICharacter->SetAnimState(CharacterAnimState::Attack);
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		// ���� ���̸� ȭ���� �غ� �� ����
		if (CharacterAnimState::AimOrBlock == AnimState && true == ArrowReady)
		{
			GlobalAICharacter->SetAnimState(CharacterAnimState::ParryorFire);
			ArrowReady = false;
		}
		// ���� ���̸� ȭ���� �غ� �ȵ� ����
		else if (CharacterAnimState::AimOrBlock == AnimState && false == EarlyArrowCheck)
		{
			EarlyArrowCheck = true;
		}
		// ������ �ƴ� �Ϲ� ������ ����
		else if (CharacterAnimState::AimOrBlock != AnimState && false == ArrowReady)
		{
			GlobalAICharacter->SetAnimState(CharacterAnimState::Attack);
		}
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState)
		{
			AttackCheck = true;
		}

		GlobalAICharacter->SetAnimState(CharacterAnimState::Attack);
	}
}

void UWeaponComponent::AimorBlockAction(float _Value)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	if (0 == _Value)
	{
		IsAimOn = false;
		IsBlock = false;

		if (CharacterAnimState::AimOrBlock == AnimState)
		{
			GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
		}

		if (EWeaponType::Bow == WeaponType && CharacterAnimState::Attack == AnimState)
		{
			return;
		}

		 UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

		if (false == IsValid(BowAnim))
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
		ReadyArrow = nullptr;

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
		GlobalAICharacter->SetAnimState(CharacterAnimState::AimOrBlock);
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		GlobalAICharacter->GetCharacterMovement()->MaxWalkSpeed = AimorBlockSpeed;
		GlobalAICharacter->SetAnimState(CharacterAnimState::AimOrBlock);

		IsAimOn = true;
	}
}

void UWeaponComponent::ParryAction()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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

	GlobalAICharacter->SetAnimState(CharacterAnimState::ParryorFire);
}

void UWeaponComponent::SetBlockSuccess(bool _Value)
{
	BlockSuccess = _Value;
}

void UWeaponComponent::SetParrySuccess(bool _Value)
{
	ParrySuccess = _Value;
}

bool UWeaponComponent::GetAttackCheck()
{
	return AttackCheck;
}

bool UWeaponComponent::GetIsAimOn()
{
	return IsAimOn;
}

bool UWeaponComponent::GetIsRollMove()
{
	return IsRollMove;
}

bool UWeaponComponent::GetIsBlock()
{
	return IsBlock;
}

bool UWeaponComponent::GetIsParry()
{
	return IsParry;
}

bool UWeaponComponent::GetBlockSuccess()
{
	return BlockSuccess;
}

bool UWeaponComponent::GetParrySuccess()
{
	return ParrySuccess;
}

bool UWeaponComponent::LockOnAfterRun()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return false;
	}

	CharacterAnimState AnimState = GetAnimState();

	if (LockOnAfterRunCount <= LockOnAfterRunTime)
	{
		IsCameraStares = false;
		LockOnAfterRunTime = 0.f;

		return true;
	}
	else if (CharacterAnimState::Attack == AnimState)
	{
		IsCameraStares = false;
		LockOnAfterRunTime = 0.f;

		return true;
	}

	return false;
}

AArrow* UWeaponComponent::GetReadyArrow()
{
	return ReadyArrow;
}

FVector UWeaponComponent::GetBowJointLocation()
{
	if (false == IsValid(BowWeaponMesh))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return FVector::ZeroVector;
	}

	FVector Pos = BowWeaponMesh->GetSocketLocation(TEXT("Bow_04_Joint"));

	return Pos;
}

EWeaponType UWeaponComponent::GetWeaponType()
{
	return WeaponType;
}

float UWeaponComponent::GetMoveXValue()
{
	return MoveXValue;
}

float UWeaponComponent::GetMoveYValue()
{
	return MoveYValue;
}

bool UWeaponComponent::GetIsLockOn()
{
	return IsLockOn;
}

bool UWeaponComponent::GetIsMove()
{
	bool IsMove = false;

	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return IsMove;
	}

	CharacterAnimState AnimState = GetAnimState();

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

bool UWeaponComponent::GetIsCameraStares()
{
	return IsCameraStares;
}

void UWeaponComponent::SetIsCameraStares(bool _Value)
{
	IsCameraStares = _Value;
}

void UWeaponComponent::GotHit(FVector _Value)
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	switch (AnimState)
	{
	case CharacterAnimState::Idle:
	case CharacterAnimState::Walk:
	case CharacterAnimState::LockOnIdle:
	case CharacterAnimState::LockOnBackward:
	case CharacterAnimState::LockOnForward:
	case CharacterAnimState::LockOnLeft:
	case CharacterAnimState::LockOnRight:
		GlobalAICharacter->SetAnimState(CharacterAnimState::GotHit);

		if (true == IsLockOn)
		{
			break;
		}

		GlobalAICharacter->SetActorRotation(_Value.ToOrientationQuat());
		break;
	case CharacterAnimState::AimOrBlock:
		if (EWeaponType::Bow == WeaponType)
		{
			GlobalAICharacter->SetAnimState(CharacterAnimState::AimOrBlockGotHit);

			UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

			if (false == IsValid(BowAnim))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return;
			}

			BowAnim->SetBowChordCheck(false);

			if (false == IsValid(ReadyArrow))
			{
				UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
				return;
			}

			ArrowReady = false;
			ReadyArrow->Destroy();
			ReadyArrow = nullptr;
		}
		else if (EWeaponType::Sword == WeaponType)
		{
			GlobalAICharacter->SetAnimState(CharacterAnimState::AimOrBlockGotHit);
		}
		break;
	default:
		break;
	}
}

void UWeaponComponent::Death()
{
	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GlobalAICharacter->SetAnimState(CharacterAnimState::Death);

	if (EWeaponType::Bow == WeaponType)
	{
		UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

		if (false == IsValid(BowAnim))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return;
		}

		BowAnim->SetBowChordCheck(false);

		if (false == IsValid(ReadyArrow))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return;
		}

		ArrowReady = false;
		ReadyArrow->Destroy();
		ReadyArrow = nullptr;
	}
}


void UWeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	
	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalGameInstance* Instance = Character->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UnArmedWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("UnArmed")));

	BackBowWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Bow")));
	BackSwordWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Sword")));
	BackShieldWeaponMesh->SetSkeletalMesh(Instance->GetWeaponMesh(TEXT("Shield")));

	TSubclassOf<UObject> Arrow = Instance->GetSubClass(TEXT("Arrow"));

	if (nullptr == Arrow)
	{
		return;
	}

	ArrowClass = Arrow;
}


void UWeaponComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	PressSpaceBarCkeckAndRoll(DeltaTime);

	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	if ((true == IsLockOn || true == IsAimOn) && (CharacterAnimState::Idle == AnimState || CharacterAnimState::Walk == AnimState))
	{
		GlobalAICharacter->SetAnimState(CharacterAnimState::LockOnIdle);
	}
	else if ((false == IsLockOn && false == IsAimOn) && CharacterAnimState::LockOnIdle == AnimState)
	{
		GlobalAICharacter->SetAnimState(CharacterAnimState::Idle);
	}

	// ���½� �޸��� �� �� �ٽ� ȸ���� �������� �ð�
	if (true == IsCameraStares)
	{
		LockOnAfterRunTime += DeltaTime;

		if (LockOnAfterRunCount <= LockOnAfterRunTime || CharacterAnimState::Attack == AnimState)
		{
			LockOnAfterRunTime = 0.f;
			IsCameraStares = false;
		}
	}

	// ȭ���� �տ� �پ��ְ�
	if (nullptr != ReadyArrow)
	{
		FVector JointPos = GetBowJointLocation();

		ReadyArrow->ArrowReRoad(GlobalAICharacter, JointPos, DeltaTime);
	}

	if (EWeaponType::Bow == WeaponType)
	{
		BowChordMove();
	}

}

void UWeaponComponent::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// ������ �ƴ� ���� üũ
	for (auto OverlapCheck : OverlapArray)
	{
		if (OverlapCheck == OtherActor)
		{
			// ������ �ƾ��ٸ� ������ ó���� ���� �ʰ� ����
			return;
		}
	}
	// ������ �� �� ���� ���͸� �߰�
	OverlapArray.Emplace(OtherActor);

	// ���� �浹 ó��
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	EWeaponType CurWeaponEnum = WeaponType;
	FName WeaponName;

	const UEnum* WeaponEnum = FindObject<UEnum>(ANY_PACKAGE, TEXT("EWeaponType"), true);

	if (0 != WeaponEnum)
	{
		FString WeaponString = WeaponEnum->GetNameStringByValue((int64)CurWeaponEnum);
		WeaponName = FName(WeaponString);
	}

	FWeaponData* WeaponData = Instance->GetWeaponData(WeaponName);

	int CurWeaponDamage = WeaponData->Damage;
	FPointDamageEvent DamageEvent;

	ACharacter* Character = Cast<ACharacter>(GetOwner());

	if (false == IsValid(Character))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	OtherActor->TakeDamage(CurWeaponDamage, DamageEvent, Character->GetController(), Character);
}

void UWeaponComponent::BowChordMove()
{
	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());
	
	if (nullptr == BowAnim)
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == BowAnim"), __FUNCTION__, __LINE__);
		return;
	}

	if (true == BowAnim->GetBowChordCheck())
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		if (false == IsValid(Character))
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
			return;
		}

		USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();

		if (nullptr == SkeletalMeshComponent)
		{
			UE_LOG(LogTemp, Error, TEXT("%S(%u)> nullptr == SkeletalMeshComponent"), __FUNCTION__, __LINE__);
			return;
		}

		FVector Vec = SkeletalMeshComponent->GetSocketLocation(TEXT("RightHandSoket"));

		BowAnim->SetHandTransform(Vec);
	}
}

CharacterAnimState UWeaponComponent::GetAnimState()
{
	CharacterAnimState AnimState = CharacterAnimState::None;

	AGlobalAICharacter* GlobalAICharacter = Cast<AGlobalAICharacter>(GetOwner());

	if (false == IsValid(GlobalAICharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return AnimState;
	}

	AnimState = static_cast<CharacterAnimState>(GlobalAICharacter->GetAnimState());

	return AnimState;
}
