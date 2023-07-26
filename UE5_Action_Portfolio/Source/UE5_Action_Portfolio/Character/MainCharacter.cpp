#include "Character/MainCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Weapon/WeaponAction.h"
#include "Weapon/BowAnimInstance.h"

AMainCharacter::AMainCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	MainCameraSpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraSpringArm"));
	MainCameraSpringArmComponent->SetupAttachment(GetCapsuleComponent());
	MainCameraSpringArmComponent->SetRelativeLocation(FVector(0.0f, 0.0f, BaseEyeHeight));
	MainCameraSpringArmComponent->bUsePawnControlRotation = true;
	MainCameraSpringArmComponent->TargetArmLength = 450.f;
	MainCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	MainCameraComponent->SetupAttachment(MainCameraSpringArmComponent, USpringArmComponent::SocketName);
	BaseTurnRate = 30.f;
	BaseLookUpRate = 30.f;

	GetCharacterMovement()->bOrientRotationToMovement = true;
}

void AMainCharacter::BeginPlay()
{
	Super::BeginPlay();

	this->bUseControllerRotationYaw = false;
}

void AMainCharacter::Tick(float _DeltaTime)
{
	Super::Tick(_DeltaTime);

	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();
	FTransform Trans;

	if (nullptr != SkeletalMeshComponent)
	{
		Trans = SkeletalMeshComponent->GetSocketTransform(TEXT("RightHand"));
		//BowWeaponMesh // SetHandTransform() 호출
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
	// 플립/플롭
	if (false == LockonChose)
	{
		
	}
	else if (true == LockonChose)
	{

	}
}
