#include "Character/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponAction.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameUI/GameUIHUD.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MainCameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	MainCameraSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight * 1.5f));
	MainCameraSpringArmComponent->bUsePawnControlRotation = true;
	MainCameraSpringArmComponent->TargetArmLength = 500.f;
	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCameraComponent->SetupAttachment(MainCameraSpringArmComponent, USpringArmComponent::SocketName);
	MainCameraComponent->SetRelativeLocation(FVector(0.0f, 55.f, 0.f));
	BaseTurnRate = 30.f;
	BaseLookUpRate = 30.f;

	FOVUpdateDelegate.BindUFunction(this, FName("AimZoomTimelineUpdate"));
	ArmUpdateDelegate.BindUFunction(this, FName("ArmTimelineUpdate"));
	TimelineFinishDelegate.BindUFunction(this, FName("AimZoomOnFinish"));

	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate.Yaw = 360.f;
	
	SetActorTypeTag(TEXT("Player"));
	SetAttackTypeTag(TEXT("PlayerAttack"));
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;

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

	SetHP(5000.f);
	SetMaxHP(GetHP());
}

void AMainCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	if (nullptr == GetCurWeaponAction())
	{
		return;
	}

	// ���� Ÿ�ٿ� ����
	LookAtTarget(_DeltaTime);

	EWeaponType CurWeponT = GetCurWeaponAction()->GetWeaponType();
	bool IsAim = GetCurWeaponAction()->GetIsAimOn();

	ChangeViewFTimeline.TickTimeline(_DeltaTime);

	APlayerController* HUDController = Cast<APlayerController>(GetController());
	AGameUIHUD* HUD = HUDController->GetHUD<AGameUIHUD>();

	if (nullptr == HUD && false == HUD->IsValidLowLevel())
	{
		return;
	}

	// ���ؽ� ī�޶� Ȯ��
	if (EWeaponType::Bow == CurWeponT && true == IsAim)
	{
		ChangeViewFTimeline.Play();

		CharTurnAim(_DeltaTime);

		HUD->GetMainWidget()->SetCrosshairOnOff(true);
	}
	else if (EWeaponType::Bow == CurWeponT && false == IsAim)
	{
		ChangeViewFTimeline.Reverse();
		HUD->GetMainWidget()->SetCrosshairOnOff(false);

		if (false == CurWeaponAction->GetIsLockOn())
		{
			this->bUseControllerRotationYaw = false;
			GetCharacterMovement()->bOrientRotationToMovement = true;
		}
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

	UPlayerInput::AddEngineDefinedActionMapping(FInputActionKeyMapping("Parry", EKeys::E));

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

	_PlayerInputComponent->BindAction("Parry", EInputEvent::IE_Pressed, this, &AMainCharacter::Parry);
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
	// ������ �� ȸ�� ����
	//if (true == CurWeaponAction->GetIsLockOn() && false == GetCurWeaponAction()->LockOnAfterRun())

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

void AMainCharacter::Parry()
{
	GetCurWeaponAction()->ParryAction();
}

void AMainCharacter::AimorBlock(float _Value)
{
	CurWeaponAction->AimorBlockAtion(_Value);
}

void AMainCharacter::LockOnTarget()
{
	// �ø�/�÷�
	if (false == CurWeaponAction->GetIsLockOn())
	{
		FVector Start = GetActorLocation(); // ���� ����
		FVector End = GetActorForwardVector() * LockOnTargetRange; // �� ����
		End = End.RotateAngleAxis(-50.f, FVector::UpVector); // ���� �������

		TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType; // ��Ʈ ������ ������Ʈ ����
		ObjectType.Emplace(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel3));
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
			CurWeaponAction->SetIsLockOn(true);
			LockedOnTargetActor = HitActor;
		}
	}
	else if (true == CurWeaponAction->GetIsLockOn())
	{
		LostLockedOnTargetActor();
	}
}

void AMainCharacter::LookAtTarget(float _DeltaTime)
{
	// ���� �ð� �� MouseInput�� false�� �Ǹ� ȭ�� ����
	if (true == MouseInput)
	{
		MouseTimeCheck += _DeltaTime;
	}

	if (3.f <= MouseTimeCheck || CharacterAnimState::Attack == GetAnimState<CharacterAnimState>())
	{
		MouseInput = false;
		MouseTimeCheck = 0.f;
		MouseX = 0.f;
		MouseY = 0.f;
	}

	if (nullptr == LockedOnTargetActor)
	{
		return;
	}
	if (nullptr == GetCurWeaponAction())
	{
		return;
	}
	if (false == GetCurWeaponAction()->GetIsLockOn())
	{
		return;
	}
	
	bool IsAim = GetCurWeaponAction()->GetIsAimOn();
	if (true == IsAim)
	{
		return;
	}
	if (true == GetCurWeaponAction()->GetLockOnCheck())
	{
		return;
	}

	APlayerController* PlayerCon = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (nullptr == PlayerCon)
	{
		return;
	}

	float MouseDeltaX = 0.f;
	float MouseDeltaY = 0.f;

	// ���콺�� �̵��� �޾ƿ�
	PlayerCon->GetInputMouseDelta(MouseDeltaX, MouseDeltaY);

	// ���밪���� ��ŭ �̵��ߴ��� Ȯ��
	MouseDeltaX = FMath::Abs(MouseDeltaX);
	MouseDeltaY = FMath::Abs(MouseDeltaY);
	

	// ���콺�� �������ٸ�
	if (0.f != MouseDeltaX || 0.f != MouseDeltaY)
	{
		MouseX += MouseDeltaX;
		MouseY += MouseDeltaY;

		MouseTimeCheck = 0.f;

		// ���콺�� ���� ��ġ �̻� �������ٸ�
		if ((8.f <= MouseX || 8.f <= MouseY) && false == MouseInput)
		{
			MouseInput = true;
			MouseX = 0.f;
			MouseY = 0.f;
		}

		UE_LOG(LogTemp, Warning, TEXT("Mouse Delta: %f, %f"), MouseDeltaX, MouseDeltaY);
		UE_LOG(LogTemp, Warning, TEXT("Mouse: %f, %f"), MouseX, MouseY);
	}
	// ���콺�� �������� �ʾҰ�, ���콺�� ���� ��ġ �̻� �������� �ʾҾ��ٸ�
	else if ((0.f == MouseDeltaX && 0.f == MouseDeltaY) && false == MouseInput)
	{
		MouseX = 0.f;
		MouseY = 0.f;

		FVector LockOnLocation = LockedOnTargetActor->GetActorLocation();

		FVector CurPos = GetActorLocation();
		CurPos.Z = 0;
		LockOnLocation.Z = 0;

		FVector Dir = LockOnLocation - CurPos;
		Dir.Normalize();

		FVector OtherForward = GetActorForwardVector();
		OtherForward.Normalize();

		float Angle0 = Dir.Rotation().Yaw;
		float Angle1 = OtherForward.Rotation().Yaw;

		if (10.f >= FMath::Abs(Angle0 - Angle1))
		{
			this->bUseControllerRotationYaw = true;
			GetCharacterMovement()->bOrientRotationToMovement = false;
		}

		FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(CurPos, LockOnLocation);
		FRotator InterpRotation = UKismetMathLibrary::RInterpTo(GetController()->GetControlRotation(), LookAtRotation, _DeltaTime, 5.f);

		FRotator LookAtActor = FRotator(InterpRotation.Pitch, InterpRotation.Yaw, GetController()->GetControlRotation().Roll);

		GetController()->SetControlRotation(LookAtActor);
		SetActorRotation(UKismetMathLibrary::RInterpTo(GetActorRotation(), LookAtRotation, _DeltaTime, 5.f));
	}
}

void AMainCharacter::CharTurnAim(float _DeltaTime)
{
	FRotator CurRot = GetTransform().Rotator();
	FRotator ControlRot = GetControlRotation();

	CurRot.Pitch = 0;
	ControlRot.Pitch = 0;

	FRotator Dir = ControlRot - CurRot;
	Dir.Normalize();

	CurRot.Normalize();
	ControlRot.Normalize();

	float Angle0 = CurRot.Yaw;
	float Angle1 = ControlRot.Yaw;

	if (5.f <= FMath::Abs(Angle0 - Angle1))
	{
		FRotator Rot = Dir * 7 * _DeltaTime;
		AddActorWorldRotation(Rot);
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		this->bUseControllerRotationYaw = true;
	}
}

void AMainCharacter::AimZoomTimelineUpdate(float _Value)
{
	// FOV�� ���� �۾�����
	MainCameraComponent->FieldOfView = _Value;
}

void AMainCharacter::ArmTimelineUpdate(FVector _Value)
{
	// ���ؽ� ī�޶� ���̰� ��¦ ��������
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight * _Value.Z));
}

void AMainCharacter::AimZoomOnFinish()
{

}

FVector AMainCharacter::CameraLineTrace()
{
	FVector TargetVector = FVector::ZeroVector;

	if (nullptr == MainCameraSpringArmComponent || nullptr == MainCameraComponent)
	{
		return TargetVector;
	}

	FHitResult HitRes;

	// ���� ������ ī�޶�κ��� �������� ��ŭ ������ ����.
	FVector StartLocationVector = MainCameraComponent->GetComponentTransform().GetTranslation();
	StartLocationVector += MainCameraComponent->GetForwardVector() * MainCameraSpringArmComponent->TargetArmLength;

	// �� ������ ī�޶�κ��� X������ �����̴�.
	FVector CameraForwardVector = MainCameraComponent->GetForwardVector();
	FVector End = StartLocationVector + (CameraForwardVector * 2000.f);

	// ���� Ʈ���̽� ä��
	ECollisionChannel Channel = ECollisionChannel::ECC_Visibility;

	FCollisionQueryParams QueryParams;

	// ���� Ʈ���̽��� �����ϴ� ���� ĳ���͸� �����Ѵ�.
	QueryParams.AddIgnoredActor(this);

	// ���� Ʈ���̽� ����
	GetWorld()->LineTraceSingleByChannel(HitRes, StartLocationVector, End, Channel, QueryParams);

	DrawDebugLine(GetWorld(), StartLocationVector, End, FColor::Red);

	if (nullptr == HitRes.GetActor())
	{
		// �� ���� ����
		TargetVector = End;
	}
	else if (nullptr != HitRes.GetActor())
	{
		// ���� Ʈ���̽��� �浹�� ����
		TargetVector = HitRes.ImpactPoint;
	}

	// ȭ���� ���ư� ������ ���Ѵ�
	FVector Joint = GetBowJointLocation();
	FVector ReturnVector = TargetVector - Joint;

	// Normalize ������Ѵ�.
	ReturnVector.Normalize();

	return ReturnVector;
}

void AMainCharacter::LostLockedOnTargetActor()
{
	LockedOnTargetActor = nullptr;
	IsLookAtTartget = false;
	MouseInput = false;
	MouseX = 0.f;
	MouseY = 0.f;
	MouseTimeCheck = 0.f;
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCurWeaponAction()->SetIsLockOn(false);
	GetCurWeaponAction()->SetLockOnCheck(false);
}