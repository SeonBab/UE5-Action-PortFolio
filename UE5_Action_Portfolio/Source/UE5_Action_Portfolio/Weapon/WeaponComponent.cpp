#include "Weapon/WeaponComponent.h"
#include "Weapon/BowAnimInstance.h"
#include "Weapon/Arrow.h"
#include "Engine/DamageEvents.h"
#include "Global/GlobalCharacter.h"
#include "Global/GlobalAnimInstance.h"
#include "Global/GlobalGameInstance.h"
#include "Global/Data/WeaponData.h"
#include "Global/Data/AnimaitionData.h"
#include "GameFramework/CharacterMovementComponent.h"

UWeaponComponent::UWeaponComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

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
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FName AttackType= GlobalCharacter->GetAttackTypeTag();

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
	// 오버랩 배열 초기화
	OverlapArray.Empty();

	// 콜리전 변경
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
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	FVector SpawnPos = GlobalCharacter->GetMesh()->GetSocketLocation(TEXT("RightHandSoket"));
	FRotator SpawnRot = { -90, 0, 0 };

	FActorSpawnParameters SParam;
	// 충돌시 근처 충돌하지 않는 위치에 스폰된다.
	SParam.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;


	if (false == IsValid(ArrowClass))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	ReadyArrow = GlobalCharacter->GetWorld()->UWorld::SpawnActor<AArrow>(ArrowClass, SpawnPos, SpawnRot, SParam);
}

void UWeaponComponent::SetCharacterAirControl(float _Value)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GlobalCharacter->GetCharacterMovement()->AirControl = _Value;
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

	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GlobalCharacter->SetIsInvincibility(false);
}

void UWeaponComponent::ChangeWeapon(FName _Weapon)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalAnimInstance* Ptr = Cast<UGlobalAnimInstance>(GlobalCharacter->GetMesh()->GetAnimInstance());

	if (false == IsValid(Ptr))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	UGlobalGameInstance* Instacne = GlobalCharacter->GetWorld()->GetGameInstance<UGlobalGameInstance>();

	const struct FAnimaitionData* FindAnimationData = Instacne->GetAllAnimaitionDatas(_Weapon);

	if (nullptr == FindAnimationData)
	{
		return;
	}

	Ptr->SetAllAnimation(FindAnimationData->AllAnimations);
	WeaponType = FindAnimationData->Type;
}

void UWeaponComponent::ChangeSetUnArmed()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	// Idle과 LockOnIdle만 무기 변경 가능, 장비를 끼고 있어야 UnArmed 가능
	if ((CharacterAnimState::Idle != AnimState && CharacterAnimState::LockOnIdle != AnimState) || EWeaponType::UnArmed == WeaponType)
	{
		return;
	}

	// 역재생을 위한 AnimInstance
	UGlobalAnimInstance* Ptr = Cast<UGlobalAnimInstance>(GlobalCharacter->GetMesh()->GetAnimInstance());

	if (false == IsValid(Ptr))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	// 착용하고 있던 장비가 활일 때
	if (EWeaponType::Bow == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		GlobalCharacter->SetAnimState(CharacterAnimState::EquipOrDisArmBow);
	}
	// 착용하고 있던 장비가 칼일 때
	else if (EWeaponType::Sword == WeaponType)
	{
		Ptr->AnimSpeed = -1.f;
		GlobalCharacter->SetAnimState(CharacterAnimState::EquipOrDisArmSwordAndShield);
	}

	ChangeWeapon(TEXT("UnArmed"));
}

void UWeaponComponent::ChangeSetBow()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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

	GlobalCharacter->SetAnimState(CharacterAnimState::EquipOrDisArmBow);

	ChangeWeapon(TEXT("Bow"));
}

void UWeaponComponent::ChangeSetSwordAndSheiled()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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

	GlobalCharacter->SetAnimState(CharacterAnimState::EquipOrDisArmSwordAndShield);

	ChangeWeapon(TEXT("SwordAndShield"));

}

void UWeaponComponent::PressSpaceBarCkeckAndRoll(float _DeltaTime)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
				Angle = GlobalCharacter->GetActorForwardVector();
				Angle.Z = 0;
			}
			if (true == IsBackwardWalk)
			{
				Angle = -GlobalCharacter->GetActorForwardVector();
				Angle.Z = 0;
			}
			if (true == IsLeftWalk)
			{
				Angle += -GlobalCharacter->GetActorRightVector();
				Angle.Z = 0;
			}
			if (true == IsRightWalk)
			{
				Angle += GlobalCharacter->GetActorRightVector();
				Angle.Z = 0;
			}

			Rotation = Angle.Rotation();
			GlobalCharacter->SetActorRotation(Rotation);
		}

		// 움직이기
		FVector DeltaLocation = GlobalCharacter->GetActorRotation().Vector();

		DeltaLocation.X = 500 * _DeltaTime;
		DeltaLocation.Y = 0;

		GlobalCharacter->AddActorLocalOffset(DeltaLocation, true);

		GlobalCharacter->SetIsInvincibility(true);
	}
}

void UWeaponComponent::WAndSButtonAction(float _Value)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
	if (nullptr != GlobalCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			GlobalCharacter->SetAnimState(CharacterAnimState::Walk);
			break;
		case CharacterAnimState::Walk:
			GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
				GlobalCharacter->SetAnimState(CharacterAnimState::Walk);
			}
			// 락온 중 달리고 난 후에는 정면으로만 걷는다.
			else if (true == LockOnCheck)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnForward);
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnBackward);
			}
			else if (1.f == _Value)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnForward);
			}
			GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
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

		const FRotator Rotation = GlobalCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::X);

		GlobalCharacter->AddMovementInput(Direction, _Value);

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
				GlobalCharacter->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == IsLockOn)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
		}
	}
}

void UWeaponComponent::DAndAButtonAction(float _Value)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
	if (nullptr != GlobalCharacter->Controller && 0.f != _Value)
	{
		switch (AnimState)
		{
		case CharacterAnimState::Idle:
			GlobalCharacter->SetAnimState(CharacterAnimState::Walk);
			break;
		case CharacterAnimState::Walk:
			GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
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
				GlobalCharacter->SetAnimState(CharacterAnimState::Walk);
			}
			// 락온 중 달리고 난 후에는 정면으로만 걷는다.
			else if (true == LockOnCheck)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnForward);
				LockOnAfterRunTime = 0.f;
			}
			else if (-1.f == _Value)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnLeft);
			}
			else if (1.f == _Value)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnRight);
			}
			GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = LockOnSpeed;
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

		const FRotator Rotation = GlobalCharacter->Controller->GetControlRotation();

		const FVector Direction = FRotationMatrix(Rotation).GetUnitAxis(EAxis::Y);

		GlobalCharacter->AddMovementInput(Direction, _Value);
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
				GlobalCharacter->SetAnimState(CharacterAnimState::Idle);
			}
			else if (true == IsLockOn)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
		}
	}
}

void UWeaponComponent::RollorRunAction(float _Value)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
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
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			else if (false == IsLockOn)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::Idle);
			}
		}

		// 짧게 입력이 들어왔는지 확인
		if (nullptr != GlobalCharacter->Controller && (0 != PressSpacebarTime && PressSpacebarTime <= RunCount))
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
			GlobalCharacter->SetAnimState(CharacterAnimState::Roll);
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
				GlobalCharacter->SetAnimState(CharacterAnimState::LockOnIdle);
			}
			else if (false == IsLockOn)
			{
				GlobalCharacter->SetAnimState(CharacterAnimState::Idle);
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

	if (nullptr != GlobalCharacter->Controller && PressSpacebarTime >= RunCount)
	{
		// 달린다
		if (true == IsLockOn)
		{
			LockOnCheck = true;
			LockOnAfterRunTime = 0.f;
			GlobalCharacter->bUseControllerRotationYaw = false;
			GlobalCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
		}

		GlobalCharacter->SetAnimState(CharacterAnimState::Run);

		GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
	}
}

void UWeaponComponent::ShiftButtonAction()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
		GlobalCharacter->SetAnimState(CharacterAnimState::RunJump);
		GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = RunSpeed;
		GlobalCharacter->Jump();
	}
	// 걸을 때, 가만히 있을 때 점프
	else
	{
		GlobalCharacter->SetAnimState(CharacterAnimState::WalkJump);
		GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
	}

	if (true == LockOnCheck)
	{
		LockOnAfterRunTime = 0.f;
	}

	GlobalCharacter->GetCharacterMovement()->AirControl = 0.5f;
}

void UWeaponComponent::AttackAction()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

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
		GlobalCharacter->SetAnimState(CharacterAnimState::Attack);
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		// 조준 중이며 화살이 준비 된 상태
		if (CharacterAnimState::AimOrBlock == AnimState && true == ArrowReady)
		{
			GlobalCharacter->SetAnimState(CharacterAnimState::ParryorFire);
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
			GlobalCharacter->SetAnimState(CharacterAnimState::Attack);
		}
	}
	else if (EWeaponType::Sword == WeaponType)
	{
		if (CharacterAnimState::Attack == AnimState)
		{
			AttackCheck = true;
		}

		GlobalCharacter->SetAnimState(CharacterAnimState::Attack);
	}
}

void UWeaponComponent::AimorBlockAtion(float _Value)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
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
			GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			GlobalCharacter->SetAnimState(CharacterAnimState::Idle);
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
		GlobalCharacter->SetAnimState(CharacterAnimState::AimOrBlock);
	}
	else if (EWeaponType::Bow == WeaponType)
	{
		GlobalCharacter->GetCharacterMovement()->MaxWalkSpeed = AimorBlockSpeed;
		GlobalCharacter->SetAnimState(CharacterAnimState::AimOrBlock);

		IsAimOn = true;
	}
}

void UWeaponComponent::ParryAction()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
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

	GlobalCharacter->SetAnimState(CharacterAnimState::ParryorFire);
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

bool UWeaponComponent::LockOnAfterRun()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return false;
	}

	CharacterAnimState AnimState = GetAnimState();

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

	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
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

bool UWeaponComponent::GetLockOnCheck()
{
	return LockOnCheck;
}

void UWeaponComponent::SetLockOnCheck(bool _Value)
{
	LockOnCheck = _Value;
}

void UWeaponComponent::GotHit(FVector _Value)
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
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
		GlobalCharacter->SetAnimState(CharacterAnimState::GotHit);

		if (true == IsLockOn)
		{
			break;
		}

		GlobalCharacter->SetActorRotation(_Value.ToOrientationQuat());
		break;
	case CharacterAnimState::AimOrBlock:
		if (EWeaponType::Bow == WeaponType)
		{
			GlobalCharacter->SetAnimState(CharacterAnimState::AimOrBlockGotHit);

			UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

			if (false == IsValid(BowAnim))
			{
				return;
			}

			BowAnim->SetBowChordCheck(false);

			if (false == IsValid(ReadyArrow))
			{
				return;
			}

			ArrowReady = false;
			ReadyArrow->Destroy();
			ReadyArrow = nullptr;
		}
		else if (EWeaponType::Sword == WeaponType)
		{
			GlobalCharacter->SetAnimState(CharacterAnimState::AimOrBlockGotHit);
		}
		break;
	default:
		break;
	}
}

void UWeaponComponent::Death()
{
	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		UE_LOG(LogTemp, Error, TEXT("%S(%u)> false == IsValid"), __FUNCTION__, __LINE__);
		return;
	}

	GlobalCharacter->SetAnimState(CharacterAnimState::Death);

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
		return;
	}

	UGlobalGameInstance* Instance = Character->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
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

	if (EWeaponType::Bow == WeaponType)
	{
		BowChordMove();
	}

	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		return;
	}

	CharacterAnimState AnimState = GetAnimState();

	if ((true == IsLockOn || true == IsAimOn) && (CharacterAnimState::Idle == AnimState || CharacterAnimState::Walk == AnimState))
	{
		GlobalCharacter->SetAnimState(CharacterAnimState::LockOnIdle);
	}
	else if ((false == IsLockOn && false == IsAimOn) && CharacterAnimState::LockOnIdle == AnimState)
	{
		GlobalCharacter->SetAnimState(CharacterAnimState::Idle);
	}

	// 락온시 달리고 난 후 다시 회전할 때까지의 시간
	if (true == LockOnCheck)
	{
		LockOnAfterRunTime += DeltaTime;

		if (LockOnAfterRunCount <= LockOnAfterRunTime || CharacterAnimState::Attack == AnimState)
		{
			LockOnAfterRunTime = 0.f;
			LockOnCheck = false;
		}
	}

	// 화살을 손에 붙어있게
	if (nullptr != ReadyArrow)
	{
		FVector JointPos = GetBowJointLocation();

		ReadyArrow->ArrowReRoad(GlobalCharacter, JointPos, DeltaTime);
	}
}

void UWeaponComponent::WeaponBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	// 오버랩 됐던 액터 체크
	for (auto OverlapCheck : OverlapArray)
	{
		if (OverlapCheck == OtherActor)
		{
			// 오버랩 됐었다면 데미지 처리를 하지 않고 리턴
			return;
		}
	}
	// 오버랩 된 적 없던 액터를 추가
	OverlapArray.Emplace(OtherActor);

	// 공격 충돌 처리
	UGlobalGameInstance* Instance = GetWorld()->GetGameInstance<UGlobalGameInstance>();

	if (false == IsValid(Instance))
	{
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
		return;
	}

	

	OtherActor->TakeDamage(CurWeaponDamage, DamageEvent, Character->GetController(), Character);
}

void UWeaponComponent::BowChordMove()
{
	UBowAnimInstance* BowAnim = Cast<UBowAnimInstance>(BowWeaponMesh->GetAnimInstance());

	if (nullptr == BowAnim)
	{
		return;
	}

	if (true == BowAnim->GetBowChordCheck())
	{
		ACharacter* Character = Cast<ACharacter>(GetOwner());

		if (false == IsValid(Character))
		{
			return;
		}

		USkeletalMeshComponent* SkeletalMeshComponent = Character->GetMesh();

		if (nullptr == SkeletalMeshComponent)
		{
			return;
		}

		FVector Vec = SkeletalMeshComponent->GetSocketLocation(TEXT("RightHandSoket"));

		BowAnim->SetHandTransform(Vec);
	}
}

CharacterAnimState UWeaponComponent::GetAnimState()
{
	CharacterAnimState AnimState = CharacterAnimState::None;

	AGlobalCharacter* GlobalCharacter = Cast<AGlobalCharacter>(GetOwner());

	if (false == IsValid(GlobalCharacter))
	{
		return AnimState;
	}

	AnimState = static_cast<CharacterAnimState>(GlobalCharacter->GetAnimState());

	return AnimState;
}
