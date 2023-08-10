#include "Character/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponAction.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MainCameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	MainCameraSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight * 1.5f));
	MainCameraSpringArmComponent->bUsePawnControlRotation = true;
	MainCameraSpringArmComponent->TargetArmLength = 1000.f;
	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCameraComponent->SetupAttachment(MainCameraSpringArmComponent, USpringArmComponent::SocketName);
	MainCameraComponent->SetRelativeLocation(FVector(0.0f, 90.f, 0.f));
	BaseTurnRate = 30.f;
	BaseLookUpRate = 30.f;

	FOVUpdateDelegate.BindUFunction(this, FName("AimZoomTimelineUpdate"));
	ArmUpdateDelegate.BindUFunction(this, FName("ArmTimelineUpdate"));
	TimelineFinishDelegate.BindUFunction(this, FName("AimZoomOnFinish"));

	GetCharacterMovement()->bOrientRotationToMovement = true;

	Tags.Add(ActorTag);
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;
	GetCurWeaponAction()->SetAttackType(AttackType);

	// Ÿ�Ӷ��� ����
	if (nullptr != FOVCurveFloat && nullptr != CameraSpringArmVector)
	{
		// Ŀ��� Ŀ�긦 ����� �Լ�
		ChangeViewFTimeline.AddInterpFloat(FOVCurveFloat, FOVUpdateDelegate);
		ChangeViewFTimeline.AddInterpVector(CameraSpringArmVector, ArmUpdateDelegate);

		// ���� �� ȣ�� �� �Լ�
		ChangeViewFTimeline.SetTimelineFinishedFunc(TimelineFinishDelegate);

		// ���� ����
		ChangeViewFTimeline.SetTimelineLength(AimZoomTimelineLength);

		// ���� ����
		ChangeViewFTimeline.SetLooping(false);
	}
}

void AMainCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (nullptr == GetCurWeaponAction())
	{
		return;
	}

	// ���� Ÿ�ٿ� ����
	if (true == CurWeaponAction->GetLockOnCheck())
	{
		LookAtTarget(_DeltaTime);
	}

	// ĳ���Ͱ� �޸��ٰ� ����ٸ� ���� �ð��� �����°�
	if (true == CurWeaponAction->LockOnAfterRun())
	{
		IsLookAtTartget = true;
	}
	// ���߰� ���� �ð��� �������� ĳ���͸� �ٽ� �������� ȸ����Ų��
	if (true == IsLookAtTartget)
	{
		FVector LockOnLocation = LockedOnTargetActor->GetActorLocation();
		FVector CurChar = GetActorLocation();

		LockOnLocation.Z = 0.0f;
		CurChar.Z = 0.0f;

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurChar, LockOnLocation);
		SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, _DeltaTime, 10.f));

		if (5.f >= FMath::Abs(LookAtRotation.Yaw - GetActorRotation().Yaw))
		{
			IsLookAtTartget = false;
			bUseControllerRotationYaw = true;
		}
	}

	EWeaponType CurWeponT = GetCurWeaponAction()->GetWeaponType();
	bool IsAim = GetCurWeaponAction()->GetIsAimOn();

	ChangeViewFTimeline.TickTimeline(_DeltaTime);

	// ���ؽ� ī�޶� Ȯ��
	if (EWeaponType::Bow == CurWeponT && true == IsAim)
	{
		ChangeViewFTimeline.Play();


		this->bUseControllerRotationYaw = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else if (EWeaponType::Bow == CurWeponT && false == IsAim && false == CurWeaponAction->GetLockOnCheck())
	{
		ChangeViewFTimeline.Reverse();
		this->bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AMainCharacter::SetupPlayerInputComponent(UInputComponent* _PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(_PlayerInputComponent);

	check(_PlayerInputComponent);

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerWheelUp", EKeys::MouseScrollUp));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerWheelDown", EKeys::MouseScrollDown));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Tab", EKeys::Tab));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerMouseLeft", EKeys::LeftMouseButton));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMouseRight", EKeys::RightMouseButton, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerTurn", EKeys::MouseX, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerLooKUp", EKeys::MouseY, -1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::W, 1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveForward", EKeys::S, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::A, -1.f));
	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerMoveRight", EKeys::D, 1.f));

	UPlayerInput::AddEngineDefinedAxisMapping(FInputAxisKeyMapping("PlayerRollorRun", EKeys::SpaceBar, 1.f));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("PlayerJump", EKeys::LeftShift));

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuickSlot1", EKeys::One));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuickSlot2", EKeys::Two));
	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("QuickSlot3", EKeys::Three));


	_PlayerInputComponent->BindAction("PlayerWheelUp", EInputEvent::IE_Pressed, this, &AMainCharacter::ZoomIn);
	_PlayerInputComponent->BindAction("PlayerWheelDown", EInputEvent::IE_Pressed, this, &AMainCharacter::ZoomOut);

	_PlayerInputComponent->BindAction("Tab", EInputEvent::IE_Pressed, this, &AMainCharacter::LockOnTarget);

	_PlayerInputComponent->BindAction("PlayerMouseLeft", EInputEvent::IE_Pressed, this, &AMainCharacter::Attack);
	_PlayerInputComponent->BindAxis("PlayerMouseRight", this, &AMainCharacter::AimorBlock);

	_PlayerInputComponent->BindAxis("PlayerTurn", this, &AMainCharacter::AddControllerYawInput);
	_PlayerInputComponent->BindAxis("PlayerLooKUp", this, &AMainCharacter::AddControllerPitchInput);

	_PlayerInputComponent->BindAxis("PlayerMoveForward", this, &AMainCharacter::MoveForward);
	_PlayerInputComponent->BindAxis("PlayerMoveRight", this, &AMainCharacter::MoveRight);

	_PlayerInputComponent->BindAxis("PlayerRollorRun", this, &AMainCharacter::RollorRun);

	_PlayerInputComponent->BindAction("PlayerJump", EInputEvent::IE_Pressed, this, &AMainCharacter::JumpAction);

	_PlayerInputComponent->BindAction("QuickSlot1", EInputEvent::IE_Pressed, this, &AMainCharacter::ChangeUnArmed);
	_PlayerInputComponent->BindAction("QuickSlot2", EInputEvent::IE_Pressed, this, &AMainCharacter::ChangeBow);
	_PlayerInputComponent->BindAction("QuickSlot3", EInputEvent::IE_Pressed, this, &AMainCharacter::ChangeSwordAndSheiled);

}

void AMainCharacter::ZoomIn()
{
	if (MainCameraSpringArmComponent->TargetArmLength > 50.f)
	{
		MainCameraSpringArmComponent->TargetArmLength *= 0.9;
	}
}

void AMainCharacter::ZoomOut()
{
	if (MainCameraSpringArmComponent->TargetArmLength < 450.f)
	{
		MainCameraSpringArmComponent->TargetArmLength *= 1.1;
	}
}

void AMainCharacter::Attack()
{
	// ���� �� �ٸ� ������ �ٶ󺸸� ���� �� �� �ٽ� ���� Ÿ�� �������� ȸ�� �� ����
	if (true == CurWeaponAction->GetLockOnCheck() && false == GetCurWeaponAction()->LockOnAfterRun())
	{
		IsLookAtTartget = true;
	}

	CurWeaponAction->AttackAction();
}

void AMainCharacter::MoveForward(float _Value)
{
	CurWeaponAction->WAndSButtonAction(_Value);
}

void AMainCharacter::MoveRight(float _Value)
{
	CurWeaponAction->DAndAButtonAction(_Value);
}

void AMainCharacter::RollorRun(float _Value)
{
	CurWeaponAction->RollorRunAction(_Value);
}

void AMainCharacter::JumpAction()
{
	CurWeaponAction->ShiftButtonAction();
}

void AMainCharacter::ChangeUnArmed()
{
	CurWeaponAction->ChangeSetUnArmed();
}

void AMainCharacter::ChangeBow()
{
	CurWeaponAction->ChangeSetBow();
}

void AMainCharacter::ChangeSwordAndSheiled()
{
	CurWeaponAction->ChangeSetSwordAndSheiled();
}

void AMainCharacter::AimorBlock(float _Value)
{
	CurWeaponAction->AimorBlockAtion(_Value);
}

void AMainCharacter::LockOnTarget()
{
	// �ø�/�÷�
	if (false == CurWeaponAction->GetLockOnCheck())
	{
		FVector Start = GetActorLocation(); // ���� ����
		FVector End = GetActorForwardVector() * LockOnTargetRange; // �� ����
		End = End.RotateAngleAxis(-50.f, FVector::UpVector); // ���� �������

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType; // ��Ʈ ������ ������Ʈ ����
		//ObjectTypeQuery3��� �ؾ��ϳ�?
		ObjectType.Emplace(EObjectTypeQuery::ObjectTypeQuery3);
		//TEnumAsByte<EObjectTypeQuery> WorldStatic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic);
		//TEnumAsByte<EObjectTypeQuery> WorldDynamic = UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic);
		//ObjectTypes.Add(WorldStatic);
		//ObjectTypes.Add(WorldDynamic);
		TArray<AActor*> NotTargeting; // ������ ���͵�
		NotTargeting.Emplace(this);

		FHitResult OutHit; // ��Ʈ ��� ���� �޴� ����

		float TargetDistance = LockOnTargetRange; // ���� ����� ���Ϳ��� �Ÿ� ����
		AActor* HitActor = nullptr; // ���� ����� ���� ����

		for (int i = 0; i <= 100; i += 10) // �ݺ� �ϸ鼭 ������ �������� ����
		{
			FVector Direction = End.RotateAngleAxis(i, FVector::UpVector);
			FVector EndPoint = Start + Direction;

			bool IsHit = UKismetSystemLibrary::SphereTraceSingleForObjects(GetWorld(), Start, EndPoint, 200.f, ObjectType, false, NotTargeting, EDrawDebugTrace::ForDuration, OutHit, true);
			
			if (true == IsHit && OutHit.Distance < TargetDistance)
			{
				TargetDistance = OutHit.Distance;
				HitActor = OutHit.GetActor();
			}
		}

		if (nullptr != HitActor)
		{
			CurWeaponAction->SetLockOnCheck(true);
			LockedOnTargetActor = HitActor;

			// ĳ���Ͱ� ���Ϳ� ������ ������
			this->bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}
	}
	else if (true == CurWeaponAction->GetLockOnCheck())
	{
		CurWeaponAction->SetLockOnCheck(false);
		LockedOnTargetActor = nullptr;
		
		// ĳ������ ���� ������ Ǯ��
		this->bUseControllerRotationYaw = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}
}

void AMainCharacter::LookAtTarget(float _DeltaTime)
{
	FVector LockOnLocation = LockedOnTargetActor->GetActorLocation();
	FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(GetActorLocation(), LockOnLocation);
	FRotator InterpRotation = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), LookAtRotation, _DeltaTime, 10.f);

	FRotator LookAtActor = FRotator(InterpRotation.Pitch, InterpRotation.Yaw, GetController()->GetControlRotation().Roll);

	GetController()->SetControlRotation(LookAtActor);
}

void AMainCharacter::AimZoomTimelineUpdate(float _Value)
{
	MainCameraComponent->FieldOfView = _Value;
}

void AMainCharacter::ArmTimelineUpdate(FVector _Value)
{
	// ���ؽ� ���̰� ��������
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight * _Value.Z));
}

void AMainCharacter::AimZoomOnFinish()
{

}